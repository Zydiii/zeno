#pragma once

#include <string>
#include <vector>
#include <zeno/extra/Resource.h>

namespace zeno {

struct RenderPassBase : IObjectClone<RenderPassBase> {
    std::string name;
    std::vector<ResourceBase*> creates;
    std::vector<ResourceBase*> reads;
    std::vector<ResourceBase*> writes;
    std::size_t refCount;
    std::size_t id;

    ZENO_API RenderPassBase(std::string const &name);
    ZENO_API virtual ~RenderPassBase();
};
}