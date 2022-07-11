#include <zeno/zeno.h>
#include <zeno/extra/RenderPass.h>
#include <zeno/extra/RenderPassBuilder.h>

namespace zeno{

struct PassData {
    texture2DResource* output1;
    texture2DResource* output2;
};

struct Pass : INode{
    virtual void apply() override {
        auto renderGraph = get_input<zeno::RenderGraphObject>("RenderGraph");
        printf("in pass %d \n", renderGraph->renderGraph.passes.size());

        auto backend = get_param<std::string>("backend");
        if(backend == "HLSL")
//            renderGraph->AddRenderPass<PassData>("Render Pass",
//                                                 [&](PassData& data, RenderPassBuilder& builder)
//                                                 {
//                                                     data.output1 = builder.create<texture2DResource>("Resource 1", texture2D());
//                                                     data.output2 = builder.create<texture2DResource>("Resource 2", texture2D());
//                                                 },
//                                                 [=](const PassData& data)
//                                                 {
//                                                     printf("in pass %d", data.output1->id);
//                                                     printf("in pass %d", data.output2->id);
//                                                 }
//                                                 );
            //auto pass = RenderPass<PassData>();
            //renderGraph->renderGraph.passes.emplace_back(std::make_shared<RenderPass<PassData>>());
            printf("HLSL \n");
        else if(backend == "GLSL")
            printf("GLSL");

        printf("in pass %d \n", renderGraph->renderGraph.passes.size());

        set_output("RenderGraph", std::move(renderGraph));
    }

};

ZENDEFNODE(Pass, {
                     {
                         {"RenderGraphObject", "RenderGraph"}
                     },
                     {
                         {"RenderGraphObject", "RenderGraph"}
                     },
                     {
                         {"enum GLSL HLSL", "backend", "GLSL"}
                     },
                     {"render graph"},
                 });
}