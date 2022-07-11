#pragma once

#include <zeno/core/INode.h>
#include <zeno/extra/RenderPass.h>
#include <zeno/extra/Resource.h>
#include <string>
#include <vector>
#include <array>

namespace zeno {

struct RenderPassBase;
struct texture2D;

struct ResourceBase {
    std::size_t id;
    std::string name;
    std::size_t refCount;

    const RenderPassBase* creator;
    std::vector<const RenderPassBase*> readers;
    std::vector<const RenderPassBase*> writers;

    ZENO_API ResourceBase(std::string const &name, RenderPassBase* const creator);
    ZENO_API virtual ~ResourceBase();

    ZENO_API virtual void instantiate() = 0;
    ZENO_API virtual void release() = 0;
};

template<class ResourceType>
struct Resource : ResourceBase {
    ResourceType type;
    std::variant<std::shared_ptr<ResourceType>, ResourceType*> resource;

    ZENO_API Resource(std::string const &name, RenderPassBase* const creator, ResourceType const &type);
    ZENO_API Resource(std::string const &name, ResourceType const &type);
    ZENO_API ~Resource();

    ZENO_API virtual void instantiate() override;
    ZENO_API virtual void release() override;
};

struct TextureResource : ResourceBase {
    std::variant<std::shared_ptr<texture2D>, texture2D*> resource;

    ZENO_API TextureResource(std::string const &name, RenderPassBase* const creator);
    ZENO_API TextureResource(std::string const &name);
    ZENO_API ~TextureResource();

    ZENO_API virtual void instantiate() override;
    ZENO_API virtual void release() override;
};


}