#pragma once

#include <zeno/core/IObject.h>
#include <zeno/types/PrimitiveObject.h>
#include <zeno/types/MaterialObject.h>
#include <zeno/extra/RenderPass.h>
#include <vector>
#include <zeno/funcs/ObjectCodec.h>

namespace zeno {
struct ResourceBase : IObject{
    std::size_t id;
    std::string name;
    std::size_t refCount;

    std::size_t creator;
    std::vector<std::size_t> readers;
    std::vector<std::size_t> writers;

    ZENO_API ResourceBase(std::string const &name, std::size_t const &creator);
    ZENO_API virtual ~ResourceBase();

    ZENO_API virtual size_t serializeSize() const {return 0;}
    ZENO_API virtual std::vector<char> serialize() const {return std::vector<char>();}
    ZENO_API virtual void serialize(char *str) const {}
    ZENO_API static ResourceBase deserialize(std::vector<char> const &str) {return ResourceBase("", 0);}
};

struct GeoResourceDataType {
    std::shared_ptr<PrimitiveObject> prim;
    std::shared_ptr<MaterialObject> mtl;

    GeoResourceDataType(std::shared_ptr<PrimitiveObject> const &prim, std::shared_ptr<MaterialObject> const &mtl)
        : prim(prim), mtl(mtl) {}

    void serialize(char *str) const{
        prim->mtl = mtl;
        std::vector<char> buf;
        encodeObject(prim.get(), buf);
        for(char c : buf)
            *str++ = c;
    };

};

struct GeoResource : ResourceBase {
    GeoResourceDataType resourceData;

    ZENO_API GeoResource(std::string const &name, std::size_t const creator);
    ZENO_API GeoResource(std::string const &name, std::shared_ptr<PrimitiveObject> const &prim, std::shared_ptr<MaterialObject> const &mtl);
    ZENO_API GeoResource(std::string const &name);
    ZENO_API ~GeoResource();

    ZENO_API size_t serializeSize() const override;
    ZENO_API std::vector<char> serialize() const override;
    ZENO_API void serialize(char *str) const override;
    ZENO_API static ResourceBase deserialize(std::vector<char> const &str);
};



}