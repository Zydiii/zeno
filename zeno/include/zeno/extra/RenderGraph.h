#pragma once

#include <zeno/core/IObject.h>
#include <zeno/extra/RenderPass.h>
#include <zeno/extra/Resource.h>
#include <zeno/extra/RenderPassBuilder.h>
#include <vector>

namespace zeno {
    struct RenderPassBase;
    struct ResourceBase;
    struct RenderPassBuilder;

    struct RenderStep {
        RenderPassBase* pass;
        std::vector<ResourceBase*> instantiatedResources;
        std::vector<ResourceBase*> releasedResources;
    };

    struct RenderGraph {
        std::size_t id;
        std::vector<std::shared_ptr<ResourceBase>> resources;
        std::vector<std::shared_ptr<RenderPassBase>> passes;
        std::vector<RenderStep> timeline;

        ZENO_API RenderGraph();
        ZENO_API ~RenderGraph();

        ZENO_API void compile();
        ZENO_API void execute() const;
        ZENO_API void clear();
        ZENO_API void debugGraphviz(std::string const &path);

        ZENO_API void AddRenderPass(std::shared_ptr<RenderPassBase> pass);
        ZENO_API void AddRetainedResource(std::shared_ptr<ResourceBase> resource);

        ZENO_API size_t serializeSize() const;
        ZENO_API std::vector<char> serialize() const;
        ZENO_API void serialize(char *str) const;
        ZENO_API void deserialize(const char *str);
        ZENO_API static RenderGraph deserialize(std::vector<char> const &str);
    };

    struct RenderGraphObject : IObjectClone<RenderGraphObject> {
        std::shared_ptr<RenderGraph> renderGraph;

        RenderGraphObject(){
            renderGraph = std::make_shared<RenderGraph>();
        }

        void AddRenderPass(std::shared_ptr<RenderPassBase> pass){
            renderGraph->AddRenderPass(pass);
        }

        void AddRetainedResource(std::shared_ptr<ResourceBase> resource){
            renderGraph->AddRetainedResource(resource);
        }

        void compile(){
            renderGraph->compile();
        }

        void execute(){
            renderGraph->execute();
        }

        void debug(){
            renderGraph->debugGraphviz("test.gv");
        }

        size_t serializeSize() const {
            size_t size{0};
            size += renderGraph->serializeSize();
            return size;
        }
        std::vector<char> serialize() const{
            std::vector<char> str;
            str.resize(serializeSize());
            serialize(str.data());
            return str;
        }
        void serialize(char *str) const{
            renderGraph->serialize(str);
        }

        void deserialize(const char *str){
            renderGraph->deserialize(str);
        }
    };

    }