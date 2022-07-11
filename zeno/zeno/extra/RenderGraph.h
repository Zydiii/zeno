#pragma once

#include <zeno/core/IObject.h>
#include <zeno/extra/RenderPass.h>
#include <zeno/extra/RenderPassBuilder.h>
#include <zeno/extra/Resource.h>
#include <vector>

namespace zeno {
    struct RenderPassBase;
    struct ResourceBase;
    struct RenderPassBuilder;
    template<class ResourceType>
    struct Resource;
    template<class DataType>
    struct RenderPass;

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

        template<class DataType, class... Arguments>
        ZENO_API RenderPass<DataType>* AddRenderPass(Arguments&&... arguments){
            passes.emplace_back(std::make_shared<RenderPass<DataType>>(arguments...));
            auto pass = passes.back().get();
            RenderPassBuilder builder(this, pass);
            pass->setup(builder);
            return static_cast<RenderPass<DataType>*>(pass);
        };
        template<class DataType>
        ZENO_API RenderPass<DataType>* AddRenderPass(){
            passes.emplace_back(std::make_shared<RenderPass<DataType>>());
            auto pass = passes.back().get();
            RenderPassBuilder builder(this, pass);
            pass->setup(builder);
            return static_cast<RenderPass<DataType>*>(pass);
        };
        template<class ResourceType>
        ZENO_API Resource<ResourceType>* AddRetainedResource(std::string const &name, ResourceType &resourceType);

    };

    struct RenderGraphObject : IObjectClone<RenderGraphObject> {
        RenderGraph renderGraph;

        template<class DataType, class... ArgsType>
        RenderPass<DataType>* AddRenderPass(ArgsType&&... arguments){
            return renderGraph.AddRenderPass<DataType>(arguments...);
        }

        void compile(){
            renderGraph.compile();
        }

        void execute(){
            renderGraph.execute();
        }
    };

    }