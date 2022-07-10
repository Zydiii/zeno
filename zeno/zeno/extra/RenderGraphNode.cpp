#include <zeno/extra/RenderGraphNode.h>
#include <stack>
#include <fstream>

namespace zeno {

ZENO_API RenderGraph::RenderGraph() = default;
ZENO_API RenderGraph::~RenderGraph() = default;

ZENO_API void RenderGraph::compile() {
    // counting ref count
    for(auto &pass : passes)
        pass->refCount = pass->creates.size() + pass->writes.size();
    std::stack<ResourceBase*> unreferencedResources;
    for(auto &resource : resources){
        resource->refCount = resource->readers.size();
        if(resource->refCount == 0)
            unreferencedResources.push(resource.get());
    }

    // delete unreferenced resource
    while(!unreferencedResources.empty()){
        auto unreferencedResource = unreferencedResources.top();
        unreferencedResources.pop();

        auto creator = const_cast<RenderPassBase*>(unreferencedResource->creator);
        if(creator->refCount > 0)
            creator->refCount--;
        if(creator->refCount == 0 && !creator->seen){
            for(auto read : creator->reads){
                auto readResource = const_cast<ResourceBase*>(read);
                if(readResource->refCount > 0)
                    readResource->refCount--;
                if(readResource->refCount == 0)
                    unreferencedResources.push(readResource);
            }
        }

        for(auto writer_ : unreferencedResource->writers){
            auto writer = const_cast<RenderPassBase*>(writer_);
            if(writer->refCount > 0)
                writer->refCount--;
            if(writer->refCount == 0 && !writer->seen){
                for(auto read : writer->reads){
                    auto readResource = const_cast<ResourceBase*>(read);
                    if(readResource->refCount > 0)
                        readResource->refCount--;
                    if(readResource->refCount == 0)
                        unreferencedResources.push(readResource);
                }
            }
        }
    }

    // compute timeline
    timeline.clear();
    for(auto &pass : passes){
        if(pass->refCount == 0 && !pass->seen)
            continue;

        std::vector<ResourceBase*> instantiatedResource, releasedResource;
        for(auto resource : pass->creates){
            instantiatedResource.push_back(const_cast<ResourceBase*>(resource));
            if(resource->readers.empty() && resource->writers.empty())
                releasedResource.push_back(const_cast<ResourceBase*>(resource));
        }

        // find the last user of the resource
        auto reads_writes = pass->reads;
        reads_writes.insert(reads_writes.end(), pass->writes.begin(), pass->writes.end());
        for(auto resource : reads_writes){
            auto valid = false;
            std::size_t lastIndex;
            if(!resource->readers.empty()){
                auto lastReader = std::find_if(passes.begin(),
                                                passes.end(),
                                                [&resource](std::unique_ptr<RenderPassBase> const &reader){
                                                    return reader.get() == resource->readers.back();
                                                });
                if(lastReader != passes.end()){
                    valid = true;
                    lastIndex = std::distance(passes.begin(), lastReader);
                }
            }

            if(!resource->writers.empty()){
                auto lastWriter = std::find_if(passes.begin(),
                                               passes.end(),
                                               [&resource](const std::unique_ptr<RenderPassBase>& writer){
                                                   return writer.get() == resource->writers.back();
                                               });
                if(lastWriter != passes.end()){
                    valid = true;
                    lastIndex = std::max(lastIndex, std::size_t(std::distance(passes.begin(), lastWriter)));
                }
            }

            if(valid && passes[lastIndex] == pass)
                releasedResource.push_back(const_cast<ResourceBase*>(resource));

            timeline.push_back(RenderStep{pass.get(), instantiatedResource, releasedResource});
        }
    }
}

ZENO_API void RenderGraph::execute() const {
    for(auto &step : timeline){
        for(auto resource : step.releasedResources)
            resource->instantiate();
        step.pass->render();
        for(auto resource : step.releasedResources)
            resource->release();
    }
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
        stream << "\"" << pass->name << "\" [label=\"" << pass->name << "\\nRefs: " << pass->refCount << "\", style=filled, fillcolor=orange]\n";
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
            stream << "\"" << pass->name << "\" ";
        stream << "} [color=forestgreen]\n";
    }
    stream << "}";
}

template <typename DataType, typename... ArgsType>
ZENO_API RenderPass<DataType> *RenderGraph::AddRenderPass(ArgsType &&...arguments) {
    passes.emplace_back(std::make_unique<RenderPass<DataType>>(arguments...));
    auto pass = passes.back().get();
    pass->setup();
    return static_cast<RenderPass<DataType>*>(pass);
}
template <typename ResourceType>
ZENO_API Resource<ResourceType> * RenderGraph::AddRetainedResource(const std::string &name,
                                                                  ResourceType &resourceType) {
    resources.emplace_back(std::make_unique<Resource<ResourceType>>(name, resourceType));
    return static_cast<Resource<ResourceType>*>(resources.back().get());
}

}