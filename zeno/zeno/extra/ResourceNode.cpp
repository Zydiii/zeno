#include <zeno/extra/ResourceNode.h>

namespace zeno {

ZENO_API ResourceBase::ResourceBase(const std::string &name, const RenderPassBase *creator)
    : name(name), creator(creator), refCount(0)
{
    static std::size_t id_{0};
    id = id_++;
}
ZENO_API ResourceBase::~ResourceBase() = default;

template <typename resourceType>
ZENO_API ResourceNode<resourceType>::ResourceNode(const std::string &name,
                                         const RenderPassBase *creator,
                                         const resourceType &type)
    : ResourceBase(name, creator), type(type) {}

template <typename ResourceType>
ZENO_API ResourceNode<ResourceType>::~ResourceNode() = default;

template <typename ResourceType>
ZENO_API void ResourceNode<ResourceType>::instantiate() {
    std::get<std::unique_ptr<ResourceType>>(resource) = std::make_unique<ResourceType>();
}

template <typename ResourceType>
ZENO_API void ResourceNode<ResourceType>::release() {
    std::get<std::unique_ptr<ResourceType>>(resource).reset();
}

}