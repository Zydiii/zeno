#pragma once

#include <zeno/core/INode.h>
#include <zeno/core/IObject.h>
#include <zeno/extra/RenderGraph.h>
#include <string>
#include <map>
#include <any>

namespace zeno {

struct RenderGraph;

struct RenderGraphObject : IObjectClone<RenderGraphObject> {
    inline static std::shared_ptr<RenderGraph> renderGraph;

    RenderGraphObject(){
        renderGraph = std::make_shared<RenderGraph>();
    }

    void AddRenderPass(std::shared_ptr<RenderPassBase> pass){
        renderGraph->AddRenderPass(pass);
    }

    void AddRetainedResource(std::shared_ptr<ResourceBase> resource){
        renderGraph->AddRetainedResource(resource);
    }

    size_t getResourceSize(){
        return renderGraph->resources.size();
    }

    size_t getRenderPassSize(){
        return renderGraph->passes.size();
    }

    void compile(){
        renderGraph->compile();
    }

    void execute(){
        renderGraph->execute();
    }

    void debug(std::string const &path){
        renderGraph->debugGraphviz(path);
    }

    size_t serializeSize() const {
        size_t size{0};
        size += renderGraph->serializeSize();
        return size;
    }
    std::vector<char> serialize() const{
        std::vector<char> str;
        str.resize(serializeSize());
        serialize(str.data());
        return str;
    }
    void serialize(char *str) const{
        renderGraph->serialize(str);
    }

    void deserialize(const char *str){
        renderGraph->deserialize(str);
    }
};

}
