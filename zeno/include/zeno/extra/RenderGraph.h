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
        std::shared_ptr<RenderPassBase> pass;
        std::vector<std::shared_ptr<ResourceBase>> instantiatedResources;
        std::vector<std::shared_ptr<ResourceBase>> releasedResources;
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


    }