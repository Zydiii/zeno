#include <zeno/zeno.h>
#include <zeno/core/INode.h>
#include <zeno/core/IObject.h>
#include <zeno/types/PrimitiveObject.h>
#include <zeno/extra/Resource.h>

namespace zeno {
struct MakeResource : INode {
    virtual void apply() override {
        auto prim = get_input<zeno::PrimitiveObject>("prim");
        auto mtl = get_input<zeno::MaterialObject>("mtl");
        auto name = get_param<std::string>("name");
        auto geoResource = std::make_shared<GeoResource>(name, prim, mtl);
        std::cout << "Resource " << geoResource->name << " with id " << geoResource->id << " has mtl " << geoResource->resourceData.mtl->mtlidkey << " has prim "  << geoResource->resourceData.prim->verts.size() << std::endl;
        //auto test = std::make_shared<zeno::NumericObject>(geoResource->id);
        set_output("resource", std::move(geoResource));
        //set_output("id", std::move(test));
    }
};

ZENDEFNODE(MakeResource, {
                             {
                                 {"primitive", "prim"},
                                 {"material", "mtl"}
                             },
                             {
                                 {"ResourceBase", "resource"}
                                 //{"int", "id"}
                             },
                             {
                                 {"string", "name", "resource"}
                             },
                             {"rendergraph"}
                         });
}