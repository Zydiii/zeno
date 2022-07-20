#include <zeno/extra/RenderPass.h>

namespace zeno {
ZENO_API RenderPassBase::RenderPassBase(std::string const &name)
    : name(name), refCount(0) 
    {
        static int id_{0};
        id = id_++;
    }

ZENO_API RenderPassBase::~RenderPassBase() = default;
}