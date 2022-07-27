#include <vector>
#include <zeno/core/INode.h>
#include <zeno/core/IObject.h>
#include <zeno/types/RenderGraphObject.h>
#include <zeno/extra/RenderPass.h>
#include <zeno/types/ListObject.h>
#include <zeno/zeno.h>

namespace zeno {
struct MakeForwardPass : INode {
    virtual void apply() override {
        auto list = get_input<zeno::ListObject>("inputs")->get<ResourceBase>();
        auto name = get_param<std::string>("name");
        auto pass = std::make_shared<ForwardPass>(name);
        auto renderGraph = get_input<zeno::RenderGraphObject>("RenderGraph");
        renderGraph->AddRenderPass(pass);
        RenderPassBuilder builder(renderGraph->renderGraph, renderGraph->renderGraph->passes.size() - 1);
        for(auto &resource : list)
        {
            if(renderGraph->getResourceSize() <= resource->id)
              renderGraph->AddRetainedResource(resource);
            builder.read(resource);
        }

        //std::cout << "RenderPass " << pass->name << " has " << pass->reads.size() << " inputs " << std::endl;
        set_output("RenderGraph", std::move(renderGraph));
        //set_output("pass", std::move(pass));
    }
};

ZENDEFNODE(MakeForwardPass, {
                             {
                                 {"ListObject", "inputs"},
                                   {"RenderGraphObject", "RenderGraph"}
                             },
                             {
                                 //{"RenderPassBase", "pass"},
                                   {"RenderGraphObject", "RenderGraph"}
                             },
                             {
                                 {"string", "name", "renderpass"}
                             },
                             {"rendergraph"}
                         });
}