// #pragma once

// #include <zeno/core/IObject.h>
// #include <zeno/extra/RenderPass.h>
// #include <zeno/types/PrimitiveObject.h>
// #include <zeno/types/MaterialObject.h>

// namespace zeno {
// struct ResourceBase : IObjectClone<ResourceBase>{
//     int id;
//     std::string name;
//     int refCount;

//     const RenderPassBase* creator;
//     //std::vector<const RenderPassBase*> readers;
//     //std::vector<const RenderPassBase*> writers;

//     ZENO_API ResourceBase(std::string const &name, RenderPassBase* const creator)
//         : name(name), creator(creator), refCount(0)
//     {
//        static int id_{0};
//        id = id_++;
//     };
//     ZENO_API ResourceBase(std::string const &name)
//         : name(name) {
//         static int id_{0};
//         id = id_++;
//     };
//     ZENO_API virtual ~ResourceBase();
// };

// struct GeoResourceDataType {
//     std::shared_ptr<PrimitiveObject> prim;
//     std::shared_ptr<MaterialObject> mtl;

//     GeoResourceDataType(std::shared_ptr<PrimitiveObject> const &prim, std::shared_ptr<MaterialObject> const &mtl)
//         : prim(prim), mtl(mtl) {}
// };

// struct GeoResource : ResourceBase {
//     GeoResourceDataType resourceData;

//     ZENO_API GeoResource(std::string const &name, RenderPassBase* const creator);
//     ZENO_API GeoResource(std::string const &name, std::shared_ptr<PrimitiveObject> const &prim, std::shared_ptr<MaterialObject> const &mtl);
//     ZENO_API GeoResource(std::string const &name);
//     ZENO_API ~GeoResource();
// };

// }