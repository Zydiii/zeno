#include <zeno/zeno.h>
#include <zeno/extra/RenderGraph.h>

namespace zeno{

struct RenderGraphBegin : INode{
    virtual void apply() override {
        auto renderGraph = std::make_shared<RenderGraphObject>();
        printf("in begin %d \n", renderGraph->renderGraph.timeline.size());
        set_output("RenderGraph", std::move(renderGraph));
    }

};

ZENDEFNODE(RenderGraphBegin, {
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
        printf("in finalize %d \n", renderGraph->renderGraph.timeline.size());
        //renderGraph->compile();
        printf("in finalize %d \n", renderGraph->renderGraph.timeline.size());
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