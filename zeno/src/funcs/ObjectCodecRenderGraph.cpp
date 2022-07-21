 #include <zeno/funcs/ObjectCodec.h>
 #include <zeno/extra/RenderGraph.h>

 namespace zeno {

 namespace _implObjectCodec {
 std::shared_ptr<RenderGraphObject> decodeRenderGraphObject(const char *it);
 std::shared_ptr<RenderGraphObject> decodeRenderGraphObject(const char *it) {
     auto obj = std::make_shared<RenderGraphObject>();
     obj->deserialize(it);
     return obj;
 }

 bool encodeRenderGraphObject(RenderGraphObject const *obj, std::back_insert_iterator<std::vector<char>> it);
 bool encodeRenderGraphObject(RenderGraphObject const *obj, std::back_insert_iterator<std::vector<char>> it) {
     auto rendergraph = obj->serialize();
     it = std::copy(rendergraph.begin(), rendergraph.end(), it);
     return true;
 }
 }

 }