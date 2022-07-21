#include <zenovis/bate/IGraphic.h>
#include <zeno/extra/RenderGraph.h>
#include <zeno/utils/log.h>

namespace zenovis {
namespace {

struct GraphicRenderGraph final : IGraphic {
    Scene *scene;

    explicit GraphicRenderGraph(Scene *scene_, zeno::RenderGraphObject *rg) : scene(scene_) {
        zeno::log_info("ToView got RenderGraphObject with content: {}", rg->renderGraph->id);
    }
};

}

void MakeGraphicVisitor::visit(zeno::RenderGraphObject *obj) {
    this->out_result = std::make_unique<GraphicRenderGraph>(this->in_scene, obj);
}
}
