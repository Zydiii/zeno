#pragma once

#include <string>
#include <vector>
#include <zeno/extra/RenderPassBuilder.h>
#include <zeno/extra/Resource.h>

namespace zeno {
    struct ResourceBase;
    struct TextureResource;
    struct RenderPassBuilder;

    struct texture2D {
        std::size_t levels;
        std::size_t format;
        std::array<std::size_t, 3> size;
    };

    //using texture2DResource = zeno::Resource<texture2D>;

    struct PassData {
        TextureResource* output1;
        TextureResource* output2;
    };

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

    template<class DataType>
    struct RenderPass : RenderPassBase {
        DataType data;

        ZENO_API RenderPass(std::string const &name, std::function<void(DataType&, RenderPassBuilder&)> setupFunc, std::function<void(DataType const&)> renderFunc);
        ZENO_API RenderPass(): RenderPassBase("") {}
        ZENO_API ~RenderPass();

        ZENO_API const std::function<void(DataType&, RenderPassBuilder&)> setupFunc;
        ZENO_API const std::function<void(DataType const&)> renderFunc;
        ZENO_API virtual void setup(RenderPassBuilder& builder) override;
        ZENO_API virtual void render() override;
    };

    template struct RenderPass<PassData>;
}