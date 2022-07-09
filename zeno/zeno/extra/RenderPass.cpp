#include <zeno/extra/RenderPass.h>

namespace zeno {
ZENO_API RenderPassBase::RenderPassBase(const std::string &name)
    : name(name), seen(false), refCount(0) {}

ZENO_API RenderPassBase::~RenderPassBase() = default;

template<typename DataType>
ZENO_API RenderPass<DataType>::RenderPass(const std::string &name, std::function<void(DataType &)> setupFunc, std::function<void(const DataType &)> renderFunc)
    : RenderPassBase(name), setupFunc(setupFunc), renderFunc(renderFunc) {}

template<typename DataType>
ZENO_API RenderPass<DataType>::~RenderPass() = default;

template<typename DataType>
ZENO_API void RenderPass<DataType>::setup() {
    setupFunc(data);
}

template<typename DataType>
ZENO_API void RenderPass<DataType>::render() {
    renderFunc(data);
}

}