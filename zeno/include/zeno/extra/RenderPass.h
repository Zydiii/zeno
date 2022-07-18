#pragma once

#include <string>
#include <vector>
#include <zeno/extra/Resource.h>

namespace zeno {
struct ResourceBase;

struct RenderPassBase {
    std::string name;
    bool isViewed;
    std::vector<ResourceBase const*> creates;
    std::vector<ResourceBase const*> reads;
    std::vector<ResourceBase const*> writes;
    std::size_t refCount;

    ZENO_API RenderPassBase(std::string const &name);
    ZENO_API virtual ~RenderPassBase();
};
}