#include <zeno/extra/RenderPassBuilder.h>

namespace zeno{
ZENO_API RenderPassBuilder::RenderPassBuilder(std::shared_ptr<RenderGraph> renderGraph, int pass)
    : renderGraph(renderGraph), pass(pass) {}
ZENO_API RenderPassBuilder::~RenderPassBuilder() = default;

ZENO_API void RenderPassBuilder::create(std::shared_ptr<ResourceBase> resource) {
    renderGraph->AddRetainedResource(resource);
    renderGraph->passes[pass]->creates.push_back(resource);
}

ZENO_API void RenderPassBuilder::read(std::shared_ptr<ResourceBase> resource) {
    //std::cout << "resource " << resource->name << std::endl;
    resource->readers.push_back(pass);
    renderGraph->passes[pass]->reads.push_back(resource);
}

ZENO_API void RenderPassBuilder::write(std::shared_ptr<ResourceBase> resource) {
    resource->writers.push_back(pass);
    renderGraph->passes[pass]->writes.push_back(resource);
}
}


