#include <zeno/extra/ResourceNode.h>

namespace zeno {

ZENO_API ResourceBase::ResourceBase(std::string const &name, RenderPassBase* const creator)
    : name(name), creator(creator), refCount(0)
{
    static std::size_t id_{0};
    id = id_++;
}

ZENO_API ResourceBase::~ResourceBase() = default;

template <typename ResourceType>
ZENO_API Resource<ResourceType>::Resource(std::string const &name, RenderPassBase* const creator, ResourceType const &type)
    : ResourceBase(name, creator), type(type) {}

template <typename ResourceType>
Resource<ResourceType>::Resource(std::string const &name, ResourceType const &type)
    : ResourceBase(name, nullptr), type(type) {}

template <typename ResourceType>
ZENO_API Resource<ResourceType>::~Resource() = default;

template <typename ResourceType>
ZENO_API void Resource<ResourceType>::instantiate() {
    std::get<std::unique_ptr<ResourceType>>(resource) = std::make_unique<ResourceType>();
}

template <typename ResourceType>
ZENO_API void Resource<ResourceType>::release() {
    std::get<std::unique_ptr<ResourceType>>(resource).reset();
}

}