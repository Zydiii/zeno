#include <zeno/zeno.h>
#include <zeno/core/INode.h>
#include <zeno/core/IObject.h>
#include <zeno/types/PrimitiveObject.h>
#include <zeno/extra/Resource.h>

namespace zeno {
struct MakeGeoResource : INode {
    virtual void apply() override {
        auto prim = get_input<zeno::PrimitiveObject>("prim");
        auto mtl = get_input<zeno::MaterialObject>("mtl");
        auto name = get_param<std::string>("name");
        auto geoResource = std::make_shared<GeoResource>(name, prim, mtl);
        //std::cout << "Resource " << geoResource->name << " with id " << geoResource->id << " has mtl " << geoResource->resourceData.mtl->mtlidkey << " has prim "  << geoResource->resourceData.prim->verts.size() << std::endl;
        set_output("resource", std::move(geoResource));
    }
};

ZENDEFNODE(MakeGeoResource, {
                             {
                                 {"primitive", "prim"},
                                 {"material", "mtl"}
                             },
                             {
                                 {"ResourceBase", "resource"}
                             },
                             {
                                 {"string", "name", "resource"}
                             },
                             {"rendergraph"}
                         });
}