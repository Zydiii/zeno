#pragma once

#include <string>
#include <zeno/core/INode.h>
#include <zeno/extra/RenderGraph.h>
#include <zeno/extra/RenderPass.h>

namespace zeno {
    struct RenderPassBase;
    struct RenderGraph;

    struct RenderPassBuilder{
        RenderGraph* renderGraph;
        RenderPassBase* pass;

        ZENO_API RenderPassBuilder(RenderGraph* renderGraph, RenderPassBase* pass);
        ZENO_API ~RenderPassBuilder();

        template <typename ResourceType, typename TypeOfResource>
        ZENO_API ResourceType* create(std::string const &name, TypeOfResource const &type);
        template <typename ResourceType>
        ZENO_API ResourceType* read(ResourceType* resource);
        template <typename ResourceType>
        ZENO_API ResourceType* write(ResourceType* resource);
    };
    }