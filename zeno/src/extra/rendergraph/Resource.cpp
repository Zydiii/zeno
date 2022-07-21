#include <zeno/extra/Resource.h>

namespace zeno {

ZENO_API ResourceBase::ResourceBase(std::string const &name, int const &creator)
   : name(name), creator(creator), refCount(0)
{
   static int id_{0};
   id = id_++;
}

ZENO_API ResourceBase::~ResourceBase() = default;

ZENO_API GeoResource::GeoResource(std::string const &name, int const creator)
    : ResourceBase(name, creator), resourceData(nullptr, nullptr) {}

ZENO_API GeoResource::GeoResource(std::string const &name, std::shared_ptr<PrimitiveObject> const &prim, std::shared_ptr<MaterialObject> const &mtl)
    : ResourceBase(name, -1), resourceData(prim, mtl) {}

ZENO_API GeoResource::GeoResource(const std::string &name)
   : ResourceBase(name, -1), resourceData(nullptr, nullptr){}

ZENO_API GeoResource::~GeoResource() = default;

ZENO_API size_t GeoResource::serializeSize() const {
    size_t size{0};

    size += sizeof(id);

    auto nameLen{name.size()};
    size += sizeof(nameLen);
    size += nameLen;

    size += sizeof(refCount);

    size += sizeof(creator);

    auto readersLen{readers.size()};
    size += sizeof(readersLen);
    for(auto &reader : readers)
    {
        size += sizeof(reader);
    }

    auto writersLen{writers.size()};
    size += sizeof(writersLen);
    for(auto &writer : writers)
    {
        size += sizeof(writer);
    }

    return size;
}

ZENO_API std::vector<char> GeoResource::serialize() const {
    std::vector<char> str;
    str.resize(serializeSize());
    serialize(str.data());
    return str;
}

ZENO_API void GeoResource::serialize(char *str) const {
    size_t i{0};

    memcpy(str + i, &id, sizeof(id));
    i += sizeof(id);

    auto nameLen{name.size()};
    memcpy(str + i, &nameLen, sizeof(nameLen));
    i += sizeof(nameLen);
    name.copy(str + i, nameLen);
    i += nameLen;

    memcpy(str + i, &refCount, sizeof(refCount));
    i += sizeof(refCount);

    memcpy(str + i, &creator, sizeof(creator));
    i += sizeof(creator);

    auto readersLen{readers.size()};
    memcpy(str + i, &readersLen, sizeof(readersLen));
    i += sizeof(readersLen);
    for(const auto &reader : readers){
        memcpy(str + i, &reader, sizeof(reader));
        i += sizeof(reader);
    }

    auto writersLen{readers.size()};
    memcpy(str + i, &writersLen, sizeof(writersLen));
    i += sizeof(writersLen);
    for(const auto &writer : writers){
        memcpy(str + i, &writer, sizeof(writer));
        i += sizeof(writer);
    }
}

}