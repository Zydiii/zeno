// #include <zeno/funcs/ObjectCodec.h>
// #include <zeno/extra/Resource.h>

// namespace zeno {

// namespace _implObjectCodec {
// std::shared_ptr<ResourceBase> decodeResourceBase(const char *it);
// std::shared_ptr<ResourceBase> decodeResourceBase(const char *it) {
//     auto mtl = std::make_shared<ResourceBase>();
//     int id = *(int *)it;
//     it += sizeof(size);
//     mtl->id = id;
//     return mtl;
// }

// bool encodeResourceBase(ResourceBase const *obj, std::back_insert_iterator<std::vector<char>> it);
// bool encodeResourceBase(ResourceBase const *obj, std::back_insert_iterator<std::vector<char>> it) {
//     auto v = obj->id;
//     std::copy(v.begin(), v.end(), it);
//     return true;
// }
// }

// }