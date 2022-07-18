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
        //auto geoResource = std::make_shared<GeoResource>(name, prim, mtl);
        auto geoResource = std::make_shared<ResourceBase>("test");
        //std::cout << "Resource " << geoResource->name << " with id " << geoResource->id << std::endl;
        set_output("resource", std::move(geoResource));
        auto test = std::make_shared<zeno::NumericObject>();
        int t = geoResource ? geoResource->id : 5;
        test->set(t);
        set_output("id", std::move(test));
    }
};

ZENDEFNODE(MakeResource, {
                             {
                                 {"primitive", "prim"},
                                 {"material", "mtl"}
                             },
                             {
                                 {"ResourceBase", "resource"},
                                 {"NumericObject", "id"}
                             },
                             {
                                 {"string", "name", "resource"}
                             },
                             {"rendergraph"}
                         });
}