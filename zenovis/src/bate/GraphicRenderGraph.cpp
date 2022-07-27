#include <zenovis/bate/IGraphic.h>
#include <zeno/types/RenderGraphObject.h>
#include <zeno/utils/log.h>

namespace zenovis {
namespace {

struct GraphicRenderGraph final : IGraphic {
    Scene *scene;

    explicit GraphicRenderGraph(Scene *scene_, zeno::RenderGraphObject *rg) : scene(scene_) {
        zeno::log_info("ToView got RenderGraphObject with passes count: {} resource count: {}",
                       rg->renderGraph->passes.size(),
                       rg->renderGraph->resources.size());
        rg->renderGraph->compile();
        //rg->renderGraph->execute();
        rg->renderGraph->debugGraphviz("test1.gv");
    }
};

}

void MakeGraphicVisitor::visit(zeno::RenderGraphObject *obj) {
    this->out_result = std::make_unique<GraphicRenderGraph>(this->in_scene, obj);
}
}
