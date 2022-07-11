#include <zeno/zeno.h>
#include <zeno/extra/RenderPass.h>

namespace zeno{

struct Pass : INode{
    virtual void apply() override {
        auto renderGraph = get_input<zeno::RenderGraphObject>("RenderGraph");
        printf("in pass %d \n", renderGraph->renderGraph.passes.size());

        auto backend = get_param<std::string>("backend");
        if(backend == "GLSL")
            renderGraph->AddRenderPass<PassData>("Render Pass",
                                                 [&](PassData& data, RenderPassBuilder& builder)
                                                 {
                                                     data.output1 = builder.create("Resource 1");
                                                     data.output2 = builder.create("Resource 2");
                                                 },
                                                 [=](const PassData& data)
                                                 {
                                                     printf("in pass %d", data.output1->id);
                                                     printf("in pass %d", data.output2->id);
                                                 }
                                                 );
            //renderGraph->renderGraph.passes.emplace_back(std::make_shared<RenderPass<PassData>>());
            //printf("GLSL \n");
        else if(backend == "HLSL")
            printf("HLSL");

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