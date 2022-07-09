#include <zeno/extra/ResourceNode.h>

namespace zeno {

ZENO_API ResourceBase::ResourceBase(const std::string &name, const RenderPass *creator)
    : name(name), creator(creator), refCount(0)
{
    static std::size_t id_{0};
    id = id_++;
}
ZENO_API ResourceBase::~ResourceBase() = default;

ZENO_API void ResourceBase::instantiate() {}

ZENO_API void ResourceBase::release() {}
}