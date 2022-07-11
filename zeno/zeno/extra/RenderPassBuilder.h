#pragma once

#include <string>
#include <zeno/core/INode.h>
#include <zeno/extra/RenderGraph.h>
#include <zeno/extra/RenderPass.h>

namespace zeno {
    struct RenderPassBase;
    struct RenderGraph;
    struct TextureResource;

    struct RenderPassBuilder{
        RenderGraph* renderGraph;
        RenderPassBase* pass;

        ZENO_API RenderPassBuilder(RenderGraph* renderGraph, RenderPassBase* pass);
        ZENO_API ~RenderPassBuilder();

        //template <class ResourceType>
        ZENO_API TextureResource* create(std::string const &name);
        //ZENO_API ResourceType* create(std::string const &name, TypeOfResource const &type);
        template <class ResourceType>
        ZENO_API ResourceType* read(ResourceType* resource);
        template <class ResourceType>
        ZENO_API ResourceType* write(ResourceType* resource);
    };
    }