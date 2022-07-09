#pragma once

#include <zeno/extra/RenderPassNode.h>
#include <zeno/extra/ResourceNode.h>
#include <vector>

namespace zeno {
    struct RenderPassBase;
    struct ResourceBase;

    struct RenderStep {
        RenderPassBase* pass;
        std::vector<ResourceBase*> instantiatedResources;
        std::vector<ResourceBase*> releasedResources;
    };

    struct RenderGraph {
        std::vector<std::unique_ptr<RenderPassBase>> passes;
        std::vector<std::unique_ptr<ResourceBase>> resources;
        std::vector<RenderStep> timeline;

        ZENO_API RenderGraph();
        ZENO_API ~RenderGraph();

        ZENO_API void compile();
        ZENO_API void execute() const;
        ZENO_API void clear();
        ZENO_API void debugGraphviz(std::string const &path);

        template<typename DataType, typename... ArgsType>
        ZENO_API RenderPass<DataType>* AddRenderPass(ArgsType&&... arguments);
        template<typename ResourceType>
        ZENO_API Resource<ResourceType>* AddRetainedResource(std::string const &name, ResourceType &resourceType);

    };
}