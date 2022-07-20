#include <zeno/zeno.h>
#include <zeno/core/INode.h>
#include <zeno/core/IObject.h>
#include <zeno/types/ListObject.h>
#include <zeno/extra/RenderPass.h>
#include <vector>

namespace zeno {
struct MakeRenderPass : INode {
    virtual void apply() override {
        auto list = get_input<zeno::ListObject>("inputs")->getRaw<ResourceBase>();
        auto name = get_param<std::string>("name");
        auto pass = std::make_shared<RenderPassBase>(name);
        //std::cout << typeid(list).name() << std::endl;
        pass->reads = list;
        std::cout << "RenderPass " << pass->name << " has " << pass->reads.size() << " inputs " << std::endl;
        set_output("pass", std::move(pass));
    }
};

ZENDEFNODE(MakeRenderPass, {
                             {
                                 {"ListObject", "inputs"}                          
                             },
                             {
                                 {"RenderPassBase", "pass"}
                             },
                             {
                                 {"string", "name", "renderpass"}
                             },
                             {"rendergraph"}
                         });
}