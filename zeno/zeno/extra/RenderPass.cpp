#include <zeno/extra/RenderPass.h>

namespace zeno {
ZENO_API RenderPass::RenderPass(const std::string &name)
    : name(name), seen(false), refCount(0) {}

ZENO_API RenderPass::~RenderPass() = default;


}