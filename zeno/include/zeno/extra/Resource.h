#pragma once

#include <zeno/core/IObject.h>
#include <zeno/types/PrimitiveObject.h>
#include <zeno/types/MaterialObject.h>

namespace zeno {
struct ResourceBase : IObject{
    int id;
    std::string name;
    int refCount;

    const int creator;

    ZENO_API ResourceBase(std::string const &name, int const &creator);
    ZENO_API virtual ~ResourceBase();
};

struct GeoResourceDataType {
    std::shared_ptr<PrimitiveObject> prim;
    std::shared_ptr<MaterialObject> mtl;

    GeoResourceDataType(std::shared_ptr<PrimitiveObject> const &prim, std::shared_ptr<MaterialObject> const &mtl)
        : prim(prim), mtl(mtl) {}
};

struct GeoResource : ResourceBase {
    GeoResourceDataType resourceData;

    ZENO_API GeoResource(std::string const &name, int const creator);
    ZENO_API GeoResource(std::string const &name, std::shared_ptr<PrimitiveObject> const &prim, std::shared_ptr<MaterialObject> const &mtl);
    ZENO_API GeoResource(std::string const &name);
    ZENO_API ~GeoResource();
};

}