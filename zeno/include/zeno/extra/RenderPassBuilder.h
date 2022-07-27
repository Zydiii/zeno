#pragma once

#include <string>
#include <zeno/core/INode.h>
#include <zeno/extra/Resource.h>
#include <zeno/extra/RenderGraph.h>
#include <zeno/extra/RenderPass.h>

namespace zeno {
struct RenderPassBase;
struct RenderGraph;
struct ResourceBase;

struct RenderPassBuilder{
    std::shared_ptr<RenderGraph> renderGraph;
    int pass;

    ZENO_API RenderPassBuilder(std::shared_ptr<RenderGraph> renderGraph, int pass);
    ZENO_API ~RenderPassBuilder();

    ZENO_API void create(std::shared_ptr<ResourceBase> resource);
    ZENO_API void read(std::shared_ptr<ResourceBase> resource);
    ZENO_API void write(std::shared_ptr<ResourceBase> resource);
};

}