#pragma once

#include <zeno/core/IObject.h>
#include <zeno/extra/RenderPass.h>
#include <zeno/extra/Resource.h>
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
        std::vector<std::shared_ptr<RenderPassBase>> passes;
        std::vector<std::shared_ptr<ResourceBase>> resources;
        std::vector<RenderStep> timeline;

        ZENO_API RenderGraph();
        ZENO_API ~RenderGraph();

        ZENO_API void compile();
        ZENO_API void execute() const;
        ZENO_API void clear();
        ZENO_API void debugGraphviz(std::string const &path);

        ZENO_API void AddRenderPass(std::shared_ptr<RenderPassBase> pass);
        ZENO_API void AddRetainedResource(std::shared_ptr<ResourceBase> resource);
    };

    struct RenderGraphObject : IObjectClone<RenderGraphObject> {
        RenderGraph renderGraph;

        void AddRenderPass(std::shared_ptr<RenderPassBase> pass){
            return renderGraph.AddRenderPass(pass);
        }

        void compile(){
            renderGraph.compile();
        }

        void execute(){
            renderGraph.execute();
        }
    };

    }