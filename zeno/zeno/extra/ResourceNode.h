#pragma once

#include <zeno/core/INode.h>
#include <zeno/extra/RenderPass.h>
#include <string>
#include <vector>

namespace zeno {

struct RenderPassBase;

struct ResourceBase {
    std::size_t id;
    std::string name;
    std::size_t refCount;

    const RenderPassBase* creator;
    std::vector<const RenderPassBase*> readers;
    std::vector<const RenderPassBase*> writers;

    ZENO_API ResourceBase(std::string const &name, RenderPassBase* const creator);
    ZENO_API ~ResourceBase();

    ZENO_API virtual void instantiate() = 0;
    ZENO_API virtual void release() = 0;
};

template<typename ResourceType>
struct ResourceNode : ResourceBase {
    ResourceType type;
    std::variant<std::unique_ptr<ResourceType>, ResourceType*> resource;

    ZENO_API ResourceNode(std::string const &name, RenderPassBase* const creator, ResourceType const &type);
    ZENO_API ~ResourceNode();

    ZENO_API virtual void instantiate() override;
    ZENO_API virtual void release() override;
};

}