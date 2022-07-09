#pragma once

#include <zeno/core/INode.h>
#include <zeno/extra/RenderPass.h>
#include <string>
#include <vector>

namespace zeno {

struct RenderPass;

struct ResourceBase {
    std::size_t id;
    std::string name;
    std::size_t refCount;

    const RenderPass* creator;
    std::vector<const RenderPass*> readers;
    std::vector<const RenderPass*> writers;

    ZENO_API ResourceBase(const std::string& name, const RenderPass* creator);
    ZENO_API ~ResourceBase();

    ZENO_API virtual void instantiate() = 0;
    ZENO_API virtual void release() = 0;
};

template<typename ResourceType>
struct ResourceNode : ResourceBase {
    ResourceType type;
    std::variant<std::unique_ptr<ResourceType>, ResourceType*> resource;

    ZENO_API ResourceNode(const std::string& name, const RenderPass* creator, const resourceType& type);
    ZENO_API ~ResourceNode();

    ZENO_API virtual void instantiate() override;
    ZENO_API virtual void release() override;
};

}