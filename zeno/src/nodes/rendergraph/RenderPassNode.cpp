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

        RenderPassBuilder builder(renderGraph->renderGraph, pass->id);
        for(auto &resource : list)
        {
            if(renderGraph->getResourceSize() <= resource->id)
              renderGraph->AddRetainedResource(resource);
            builder.read(resource);
        }
        std::string outputname = get_param<std::string>("outputname") + std::to_string(pass->id);
        std::shared_ptr<TextureResource> texResource = std::make_shared<TextureResource>(outputname, pass->id);
        //std::cout << "texResource->id" << texResource->id << std::endl;
        //std::cout << "pass->id" << pass->id << std::endl;
        //builder.create(texResource);
        //std::cout << renderGraph->getRenderPassSize() << " " << renderGraph->getResourceSize() << std::endl;
        renderGraph->AddRetainedResource(texResource);
        builder.write(texResource);

        //std::cout << "RenderPass " << pass->name << " has " << pass->reads.size() << " inputs " << std::endl;
        set_output("output", std::move(texResource));
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
                                 {"ResourceBase", "output"},
                                 {"RenderGraphObject", "RenderGraph"}
                             },
                             {
                                 {"string", "name", "ForwardPass"},
                                 {"string", "outputname", "ForwardPassOutput"}
                             },
                             {"rendergraph"}
                         });
}