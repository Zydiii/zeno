#pragma once

#include <zeno/extra/ResourceNode.h>
#include <string>
#include <vector>

namespace zeno {
    struct ResourceBase;

    struct RenderPassBase {
        std::string name;
        bool seen;
        std::vector<ResourceBase const*> creates;
        std::vector<ResourceBase const*> reads;
        std::vector<ResourceBase const*> writes;
        std::size_t refCount;

        ZENO_API RenderPassBase(std::string const &name);
        ZENO_API virtual ~RenderPassBase();

        ZENO_API virtual void setup() = 0;
        ZENO_API virtual void render() = 0;
    };

    template<typename DataType>
    struct RenderPass : RenderPassBase {
        DataType data;

        ZENO_API RenderPass(std::string const &name, std::function<void(DataType&)> setupFunc, std::function<void(DataType const&)> renderFunc);
        ZENO_API ~RenderPass();

        ZENO_API const std::function<void(DataType&)> setupFunc;
        ZENO_API const std::function<void(DataType const&)> renderFunc;
        ZENO_API virtual void setup() override;
        ZENO_API virtual void render() override;
    };
}