 #include <zeno/funcs/ObjectCodec.h>
 #include <zeno/extra/RenderGraph.h>

 namespace zeno {

 namespace _implObjectCodec {
 std::shared_ptr<RenderGraphObject> decodeRenderGraphObject(const char *it);
 std::shared_ptr<RenderGraphObject> decodeRenderGraphObject(const char *it) {
     auto obj = std::make_shared<RenderGraphObject>();
     std::size_t id = *(std::size_t *)it;
     it += sizeof(id);
     obj->renderGraph->id = id;
     return obj;
 }

 bool encodeRenderGraphObject(RenderGraphObject const *obj, std::back_insert_iterator<std::vector<char>> it);
 bool encodeRenderGraphObject(RenderGraphObject const *obj, std::back_insert_iterator<std::vector<char>> it) {
     auto id = obj->renderGraph->id;
     it = std::copy_n((char const *)&id, sizeof(id), it);
     return true;
 }
 }

 }