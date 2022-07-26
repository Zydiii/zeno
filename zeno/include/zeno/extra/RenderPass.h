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

    ZENO_API RenderPassBase();
    ZENO_API RenderPassBase(std::string const &name);
    ZENO_API virtual ~RenderPassBase();

    ZENO_API virtual size_t serializeSize() const {return 0;}
    ZENO_API virtual std::vector<char> serialize() const {return std::vector<char>();}
    ZENO_API virtual void serialize(char *str) const {}
    ZENO_API static RenderPassBase deserialize(std::vector<char> const &str) {return RenderPassBase();}
    ZENO_API virtual void render(){};
};

struct GeoPass : RenderPassBase {
    ZENO_API GeoPass();
    ZENO_API GeoPass(std::string const &name);
    ZENO_API ~GeoPass();

    ZENO_API size_t serializeSize() const override;
    ZENO_API std::vector<char> serialize() const override;
    ZENO_API void serialize(char *str) const override;
    ZENO_API static GeoPass deserialize(std::vector<char> const &str);
    ZENO_API virtual void render() override;
};

}