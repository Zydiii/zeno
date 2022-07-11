#include <zeno/extra/RenderPassBuilder.h>

namespace zeno{
ZENO_API RenderPassBuilder::RenderPassBuilder(RenderGraph* renderGraph, RenderPassBase* pass)
    : renderGraph(renderGraph), pass(pass) {}
ZENO_API RenderPassBuilder::~RenderPassBuilder() = default;

template <typename ResourceType, typename TypeOfResource>
ResourceType *RenderPassBuilder::create(std::string const &name, TypeOfResource const &type) {
    renderGraph->resources.emplace_back(std::make_shared<ResourceType>(name, pass, type));
    const auto resource = renderGraph->resources.back().get();
    pass->creates.push_back(resource);
    return static_cast<ResourceType*>(resource);
}

template <typename ResourceType>
ResourceType *RenderPassBuilder::read(ResourceType *resource) {
    resource->readers.push_back(pass);
    pass->reads.push_back(resource);
    return resource;
}

template <typename ResourceType>
ResourceType *RenderPassBuilder::write(ResourceType *resource) {
    resource->writers.push_back(pass);
    pass->writes.push_back(resource);
    return resource;
}

}


