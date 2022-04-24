#include <zeno/utils/vec.h>
#include <zeno/types/MapStablizer.h>
#include <zeno/types/UserData.h>
#include <zenovis/Camera.h>
#include <zenovis/IGraphic.h>
#include <zenovis/Scene.h>
#include <zenovis/ShaderManager.h>
#include <zenovis/makeGraphic.h>
#include <zenovis/opengl/buffer.h>
#include <zenovis/opengl/shader.h>
#include <zeno/types/LightObject.h>
#include <zenovis/Light.h>

namespace zenovis {
namespace {

struct GraphicLight : IGraphic {
    Scene *scene;

    explicit GraphicLight(Scene *scene_, std::shared_ptr<zeno::LightObject> const &lit) : scene(scene_) {
        auto nodeid = lit->userData().get("ident");
        scene->lightCluster->addLight(static_cast<zeno::LightData const &>(*lit));
        // TODO: implement modify scene->light
    }

    virtual void draw(bool reflect, bool depthPass) override {
    }

    virtual void drawShadow(Light *light) override {
    }
};

}

std::unique_ptr<IGraphic> makeGraphicLight(Scene *scene, std::shared_ptr<zeno::IObject> obj) {
    if (auto lit = std::dynamic_pointer_cast<zeno::LightObject>(obj))
        return std::make_unique<GraphicLight>(scene, std::move(lit));
    return nullptr;
}

}