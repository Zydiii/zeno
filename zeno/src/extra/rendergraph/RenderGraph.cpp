#include <fstream>
#include <stack>
#include <zeno/extra/RenderGraph.h>

namespace zeno {

ZENO_API RenderGraph::RenderGraph()
{
    static int id_{10};
    id = id_++;
}
ZENO_API RenderGraph::~RenderGraph() = default;

ZENO_API void RenderGraph::compile() {
    // counting ref count
    for(auto &pass : passes)
        pass->refCount = pass->creates.size() + pass->writes.size();
    for(auto &pass : passes)
        std::cout << "pass " << pass->id << " has " << pass->refCount << std::endl;
    std::stack<ResourceBase*> unreferencedResources;
    for(auto &resource : resources){
        resource->refCount = resource->readers.size();
        if(resource->refCount == 0)
            unreferencedResources.push(resource.get());
        std::cout << "resource " << resource << "has readers " << resource->refCount << std::endl;
    }

    // delete unreferenced resource
    while(!unreferencedResources.empty()){
        auto unreferencedResource = unreferencedResources.top();
        unreferencedResources.pop();

        auto creator = passes[unreferencedResource->creator];
        if(creator->refCount > 0)
            creator->refCount--;
        if(creator->refCount == 0){
            for(auto read : creator->reads){
                auto readResource = read;
                if(readResource->refCount > 0)
                    readResource->refCount--;
                if(readResource->refCount == 0)
                    unreferencedResources.push(readResource.get());
            }
        }

        for(auto writer_ : unreferencedResource->writers){
            auto writer = passes[writer_];
            if(writer->refCount > 0)
                writer->refCount--;
            if(writer->refCount == 0){
                for(auto read : writer->reads){
                    auto readResource = read;
                    if(readResource->refCount > 0)
                        readResource->refCount--;
                    if(readResource->refCount == 0)
                        unreferencedResources.push(readResource.get());
                }
            }
        }
    }

    // compute timeline
    timeline.clear();
    for(auto &pass : passes){
//        if(pass->refCount == 0)
//            continue;

        std::vector<ResourceBase*> instantiatedResource, releasedResource;
        for(auto resource : pass->creates){
            instantiatedResource.push_back(resource.get());
            if(resource->readers.empty() && resource->writers.empty())
                releasedResource.push_back(resource.get());
        }

        // find the last user of the resource
        auto reads_writes = pass->reads;
        reads_writes.insert(reads_writes.end(), pass->writes.begin(), pass->writes.end());
        for(auto resource : reads_writes){
            auto valid = false;
            size_t lastIndex;
            if(!resource->readers.empty() && resource->readers.back() < passes.size()){
                valid = true;
                lastIndex = resource->readers.back();
            }

            if(!resource->writers.empty() && resource->writers.back() < passes.size()){
                valid = true;
                lastIndex = std::max(lastIndex, resource->writers.back());
            }

            if(valid && passes[lastIndex] == pass)
                releasedResource.push_back(resource.get());

            timeline.push_back(RenderStep{pass.get(), instantiatedResource, releasedResource});
        }
    }
}

ZENO_API void RenderGraph::execute() const {
    // for(auto &step : timeline){
    //     for(auto resource : step.releasedResources)
    //         resource->instantiate();
    //     step.pass->render();
    //     for(auto resource : step.releasedResources)
    //         resource->release();
    // }
}

ZENO_API void RenderGraph::clear() {
    passes.clear();
    resources.clear();
}

ZENO_API void RenderGraph::debugGraphviz(std::string const &path) {
    std::ofstream stream(path);
    stream << "digraph framegraph \n{\n";
    stream << "rankdir = LR\n";
    stream << "bgcolor = white\n\n";
    stream << "node [shape=rectangle, fontname=\"Times-Roman\", fontsize=12]\n\n";

    // render pass
    for (auto& pass : passes)
        stream << "\"" << pass->name << "\" [label=\"" << pass->name << "\\nRefs: " << pass->refCount << "\\nID: " << pass->id << "\", style=filled, fillcolor=orange]\n";
    stream << "\n";

    // resource
    for (auto& resource : resources)
        stream << "\"" << resource->name << "\" [label=\"" << resource->name << "\\nRefs: " << resource->refCount << "\\nID: " << resource->id << "\", style=filled, fillcolor=skyblue4" << "]\n";
    stream << "\n";

    for (auto& pass : passes)
    {
        // create
        stream << "\"" << pass->name << "\" -> { ";
        for (auto& resource : pass->creates)
            stream << "\"" << resource->name << "\" ";
        stream << "} [color=firebrick]\n";

        // write
        stream << "\"" << pass->name << "\" -> { ";
        for (auto& resource : pass->writes)
            stream << "\"" << resource->name << "\" ";
        stream << "} [color=deeppink]\n";
    }
    stream << "\n";

    // read
    for (auto& resource : resources)
    {
        stream << "\"" << resource->name << "\" -> { ";
        for (auto& pass : resource->readers)
            stream << "\"" << passes[pass]->name << "\" ";
        stream << "} [color=forestgreen]\n";
    }
    stream << "}";
}

ZENO_API void RenderGraph::AddRenderPass(std::shared_ptr<RenderPassBase> pass){
    passes.emplace_back(pass);
};

ZENO_API void RenderGraph::AddRetainedResource(std::shared_ptr<ResourceBase> resource) {
    resources.emplace_back(resource);
}

ZENO_API size_t RenderGraph::serializeSize() const {
    size_t size{0};

    size += sizeof(id);

    auto resourcesLen(resources.size());
    size += sizeof(resourcesLen);
    for(auto &resource : resources)
    {
        auto resourceStrSize = resource->actualSerializeSize();

        size += sizeof(resourceStrSize);
        size += resourceStrSize;
    }

    auto passesLen(passes.size());
    size += sizeof(passesLen);
    for(auto &pass : passes)
    {
        auto passStrSize = pass->serializeSize();
        size += sizeof(passStrSize);
        size += passStrSize;
    }

    return size;
}

ZENO_API std::vector<char> RenderGraph::serialize() const {
    std::vector<char> str;
    str.resize(serializeSize());
    serialize(str.data());
    return str;
}

ZENO_API void RenderGraph::serialize(char *str) const {
    size_t i{0};

    memcpy(str + i, &id, sizeof(id));
    i += sizeof(id);

    auto resourcesLen{resources.size()};
    memcpy(str + i, &resourcesLen, sizeof(resourcesLen));
    i += sizeof(resourcesLen);

    for(const auto &resource : resources){
        auto resourceStr = resource->serialize();
        auto resourceStrSize = resourceStr.size();

        memcpy(str + i, &resourceStrSize, sizeof(resourceStrSize));
        i += sizeof(resourceStrSize);

        memcpy(str + i, resourceStr.data(), resourceStrSize);
        i += resourceStrSize;

        std::cout << "resource done " << resourceStrSize << std::endl;
    }

    std::cout << "resource done -- " << std::endl;

    auto passesLen{passes.size()};
    memcpy(str + i, &passesLen, sizeof(passesLen));
    i += sizeof(passesLen);
    for(const auto &pass : passes){
        auto passStr = pass->serialize();
        auto passStrSize = passStr.size();

        memcpy(str + i, &passStrSize, sizeof(passStrSize));
        i += sizeof(passStrSize);

        memcpy(str + i, passStr.data(), passStrSize);
        i += passStrSize;
    }
}

ZENO_API void RenderGraph::deserialize(const char *str) {
    size_t i{0};

    memcpy(&id, str + i, sizeof(id));
    i += sizeof(id);

    size_t resourcesLen;
    memcpy(&resourcesLen, str + i, sizeof(resourcesLen));
    i += sizeof(resourcesLen);
    this->resources.resize(resourcesLen);

    for(size_t j{0}; j < resourcesLen; ++j){
        size_t resourceLen;
        memcpy(&resourceLen, str + i, sizeof(resourceLen));
        i += sizeof(resourceLen);

        std::vector<char> resourceStr;
        resourceStr.resize(resourceLen);
        memcpy(resourceStr.data(), str + i, resourceLen);
        i += resourceLen;

        auto resource = std::make_shared<GeoResource>(GeoResource::deserialize(resourceStr));

        this->resources[j] = resource;

        //auto res = std::static_pointer_cast<GeoResource>(resource);
        std::cout << "res " << resource->name << " with id " << resource->id << " hast mtl " << resource->resourceData.mtl->mtlidkey << " has prim "  << resource->resourceData.prim->verts.size() << std::endl;
    }


    size_t passesLen;
    memcpy(&passesLen, str + i, sizeof(passesLen));
    i += sizeof(passesLen);
    this->passes.resize(passesLen);
    for(size_t j{0}; j < passesLen; ++j){
        size_t passLen;
        memcpy(&passLen, str + i, sizeof(passLen));
        i += sizeof(passLen);

        std::vector<char> passStr;
        passStr.resize(passLen);
        memcpy(passStr.data(), str + i, passLen);
        i += passLen;

        auto pass = std::make_shared<RenderPassBase>(RenderPassBase::deserialize(passStr));
        this->passes[j] = pass;
    }
}

ZENO_API RenderGraph RenderGraph::deserialize(const std::vector<char> &str) {
    RenderGraph renderGraph;
    renderGraph.deserialize(str.data());
    return renderGraph;
}
}