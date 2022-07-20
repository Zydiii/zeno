#pragma once

#include <string>
#include <vector>
#include <zeno/extra/Resource.h>

namespace zeno {
struct ResourceBase;

struct RenderPassBase : IObjectClone<RenderPassBase> {
    std::string name;
    std::vector<std::shared_ptr<ResourceBase>> creates;
    std::vector<std::shared_ptr<ResourceBase>> reads;
    std::vector<std::shared_ptr<ResourceBase>> writes;
    std::size_t refCount;
    std::size_t id;

    ZENO_API RenderPassBase(std::string const &name);
    ZENO_API virtual ~RenderPassBase();
};
}