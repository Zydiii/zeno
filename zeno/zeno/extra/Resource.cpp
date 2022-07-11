#include <zeno/extra/Resource.h>

namespace zeno {

ZENO_API ResourceBase::ResourceBase(std::string const &name, RenderPassBase* const creator)
    : name(name), creator(creator), refCount(0)
{
    static std::size_t id_{0};
    id = id_++;
}

ZENO_API ResourceBase::~ResourceBase() = default;

template <class ResourceType>
ZENO_API Resource<ResourceType>::Resource(std::string const &name, RenderPassBase* const creator, ResourceType const &type)
    : ResourceBase(name, creator), type(type) {}

template <class ResourceType>
Resource<ResourceType>::Resource(std::string const &name, ResourceType const &type)
    : ResourceBase(name, nullptr), type(type) {}

template <class ResourceType>
ZENO_API Resource<ResourceType>::~Resource() = default;

template <class ResourceType>
ZENO_API void Resource<ResourceType>::instantiate() {
    std::get<std::shared_ptr<ResourceType>>(resource) = std::make_shared<ResourceType>();
}

template <class ResourceType>
ZENO_API void Resource<ResourceType>::release() {
    std::get<std::shared_ptr<ResourceType>>(resource).reset();
}

ZENO_API TextureResource::TextureResource(std::string const &name, RenderPassBase* const creator)
    : ResourceBase(name, creator) {}

TextureResource::TextureResource(std::string const &name)
    : ResourceBase(name, nullptr) {}

ZENO_API TextureResource::~TextureResource() = default;

ZENO_API void TextureResource::instantiate() {
    std::get<std::shared_ptr<texture2D>>(resource) = std::make_shared<texture2D>();
}

ZENO_API void TextureResource::release() {
    std::get<std::shared_ptr<texture2D>>(resource).reset();
}

}