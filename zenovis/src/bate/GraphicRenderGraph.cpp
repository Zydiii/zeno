#include <zenovis/bate/IGraphic.h>
#include <zeno/types/RenderGraphObject.h>
#include <zeno/utils/log.h>

namespace zenovis {
namespace {

struct GraphicRenderGraph final : IGraphic {
    Scene *scene;

    explicit GraphicRenderGraph(Scene *scene_, zeno::RenderGraphObject *rg) : scene(scene_) {
        zeno::log_info("ToView got RenderGraphObject with passes count: {} resource count: {}",
                       rg->getRenderPassSize(),
                       rg->getResourceSize());
        //rg->compile();
        //std::string path = "rendergraph_zevis" + std::to_string(rg->renderGraph->id) + ".gv";
        //rg->debug(path);
        //rg->renderGraph->execute();
    }
};

}

void MakeGraphicVisitor::visit(zeno::RenderGraphObject *obj) {
    this->out_result = std::make_unique<GraphicRenderGraph>(this->in_scene, obj);
}
}
