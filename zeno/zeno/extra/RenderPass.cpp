#include <zeno/extra/RenderPass.h>
#include <zeno/extra/Resource.h>

namespace zeno {
ZENO_API RenderPassBase::RenderPassBase(std::string const &name)
    : name(name), seen(false), refCount(0) {}

ZENO_API RenderPassBase::~RenderPassBase() = default;

template<class DataType>
ZENO_API RenderPass<DataType>::RenderPass(std::string const &name, std::function<void(DataType&, RenderPassBuilder&)> setupFunc, std::function<void(DataType const&)> renderFunc)
    : RenderPassBase(name), setupFunc(setupFunc), renderFunc(renderFunc) {}

template<class DataType>
ZENO_API RenderPass<DataType>::~RenderPass() = default;

template<class DataType>
ZENO_API void RenderPass<DataType>::setup(RenderPassBuilder& builder) {
    setupFunc(data, builder);
}

template<class DataType>
ZENO_API void RenderPass<DataType>::render() {
    renderFunc(data);
}

}