#pragma once

#include "ResourceNode.h"
#include <string>
#include <vector>
#include <zeno/extra/ResourceNode.h>

namespace zeno {
    struct ResourceBase;

    struct RenderPass {
        std::string name;
        bool seen;
        std::vector<const ResourceBase*> creates;
        std::vector<const ResourceBase*> reads;
        std::vector<const ResourceBase*> writes;
        std::size_t refCount;

        ZENO_API RenderPass(const std::string& name);
        ZENO_API ~RenderPass();

        ZENO_API virtual void setup() = 0;
        ZENO_API virtual void render() = 0;
    };
}