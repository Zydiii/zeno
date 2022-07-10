#pragma once

#include <zeno/extra/RenderPassBuilder.h>
#include <zeno/extra/ResourceNode.h>
#include <string>
#include <vector>

namespace zeno {
    struct ResourceBase;
    struct RenderPassBuilder;

    struct RenderPassBase {
        std::string name;
        bool seen;
        std::vector<ResourceBase const*> creates;
        std::vector<ResourceBase const*> reads;
        std::vector<ResourceBase const*> writes;
        std::size_t refCount;

        ZENO_API RenderPassBase(std::string const &name);
        ZENO_API virtual ~RenderPassBase();

        ZENO_API virtual void setup(RenderPassBuilder& builder) = 0;
        ZENO_API virtual void render() = 0;
    };

    template<typename DataType>
    struct RenderPass : RenderPassBase {
        DataType data;

        ZENO_API RenderPass(std::string const &name, std::function<void(DataType&, RenderPassBuilder&)> setupFunc, std::function<void(DataType const&)> renderFunc);
        ZENO_API ~RenderPass();

        ZENO_API const std::function<void(DataType&, RenderPassBuilder&)> setupFunc;
        ZENO_API const std::function<void(DataType const&)> renderFunc;
        ZENO_API virtual void setup(RenderPassBuilder& builder) override;
        ZENO_API virtual void render() override;
    };
}