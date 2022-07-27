#include <zeno/types/RenderGraphObject.h>
#include <zeno/zeno.h>

namespace zeno{

struct MakeRenderGraph : INode{
    virtual void apply() override {
        auto renderGraph = std::make_shared<RenderGraphObject>();
        //printf("in begin %d \n", renderGraph->renderGraph->timeline.size());
        set_output("RenderGraph", std::move(renderGraph));
    }

};

ZENDEFNODE(MakeRenderGraph, {
                                 {},
                                 {
                                     {"RenderGraphObject", "RenderGraph"}
                                 },
                                 {},
                                 {"rendergraph"},
                             });

struct RenderGraphFinalize : INode{
    virtual void apply() override {
        auto renderGraph = get_input<zeno::RenderGraphObject>("RenderGraph");
        renderGraph->compile();
        //printf("in finalize %d \n", renderGraph->renderGraph->timeline.size());
        //renderGraph->debug();
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
                                    {"rendergraph"},
                                });
}