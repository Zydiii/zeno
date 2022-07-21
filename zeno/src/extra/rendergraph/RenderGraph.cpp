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
            int lastIndex;
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

    auto passesLen(passes.size());
    size += sizeof(passesLen);
    for(auto &pass : passes)
    {
        auto passStrSize = pass->serializeSize();
        size += sizeof(passStrSize);
        size += passStrSize;
    }

    auto resourcesLen(resources.size());
    size += sizeof(resourcesLen);
    for(auto &resource : resources)
    {
        auto resourceStrSize = resource->serializeSize();
        size += sizeof(resourceStrSize);
        size += resourceStrSize;
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

    auto passesLen{passes.size()};
    memcpy(str + i, &passesLen, sizeof(passesLen));
    i += sizeof(passesLen);
    for(const auto &pass : passes){
        memcpy(str + i, &pass, sizeof(pass));
        i += sizeof(pass);
    }

    auto resourcesLen{resources.size()};
    memcpy(str + i, &resourcesLen, sizeof(resourcesLen));
    i += sizeof(resourcesLen);
    for(const auto &resource : resources){
        memcpy(str + i, &resource, sizeof(resource));
        i += sizeof(resource);
    }
}

}