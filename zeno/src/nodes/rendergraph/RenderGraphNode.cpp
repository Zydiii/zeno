#include <zeno/extra/RenderGraph.h>
#include <zeno/zeno.h>

namespace zeno{

struct MakeRenderGraph : INode{
    virtual void apply() override {
        auto renderGraph = std::make_shared<RenderGraphObject>();
        printf("in begin %d \n", renderGraph->renderGraph.timeline.size());
        set_output("RenderGraph", std::move(renderGraph));
    }

};

ZENDEFNODE(MakeRenderGraph, {
                                 {},
                                 {
                                     {"RenderGraphObject", "RenderGraph"}
                                 },
                                 {},
                                 {"render graph"},
                             });

struct RenderGraphFinalize : INode{
    virtual void apply() override {
        auto renderGraph = get_input<zeno::RenderGraphObject>("RenderGraph");
        printf("in finalize %d \n", renderGraph->renderGraph.passes.size());
        renderGraph->compile();
        printf("in finalize %d \n", renderGraph->renderGraph.passes.size());
        set_output("RenderGraph", std::move(renderGraph));
    }

};

ZENDEFNODE(RenderGraphFinalize, {
                                    {
                                        {"RenderGraphObject", "RenderGraph"}
                                    },
                                    {
                                        {"RenderGraphObject", "RenderGraph"}
                                    },
                                    {},
                                    {"render graph"},
                                });
}