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

    ZENO_API void instantiate();
    ZENO_API void release();
};

}