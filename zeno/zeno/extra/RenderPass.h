#pragma once

#include <zeno/extra/ResourceNode.h>
#include <string>
#include <vector>

namespace zeno {
    struct ResourceBase;

    struct RenderPassBase {
        std::string name;
        bool seen;
        std::vector<const ResourceBase*> creates;
        std::vector<const ResourceBase*> reads;
        std::vector<const ResourceBase*> writes;
        std::size_t refCount;

        ZENO_API RenderPassBase(const std::string& name);
        ZENO_API ~RenderPassBase();

        ZENO_API virtual void setup() = 0;
        ZENO_API virtual void render() = 0;
    };

    template<typename DataType>
    struct RenderPass : RenderPassBase {
        DataType data;

        ZENO_API RenderPass(const std::string& name, std::function<void(DataType&)> setupFunc, std::function<void(const DataType&)> renderFunc);
        ZENO_API ~RenderPass();

        ZENO_API const std::function<void(DataType&)> setupFunc;
        ZENO_API const std::function<void(const DataType&)> renderFunc;
        ZENO_API virtual void setup() override;
        ZENO_API virtual void render() override;
    };
}