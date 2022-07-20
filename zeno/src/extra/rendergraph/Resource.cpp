#include <zeno/extra/Resource.h>

namespace zeno {

ZENO_API ResourceBase::ResourceBase(std::string const &name, int const &creator)
   : name(name), creator(creator), refCount(0)
{
   static int id_{0};
   id = id_++;
}

ZENO_API ResourceBase::~ResourceBase() = default;

ZENO_API GeoResource::GeoResource(std::string const &name, int const creator)
    : ResourceBase(name, creator), resourceData(nullptr, nullptr) {}

GeoResource::GeoResource(std::string const &name, std::shared_ptr<PrimitiveObject> const &prim, std::shared_ptr<MaterialObject> const &mtl)
    : ResourceBase(name, -1), resourceData(prim, mtl) {}

GeoResource::GeoResource(const std::string &name)
   : ResourceBase(name, -1), resourceData(nullptr, nullptr){}

ZENO_API GeoResource::~GeoResource() = default;
}