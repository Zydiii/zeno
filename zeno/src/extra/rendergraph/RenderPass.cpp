#include <zeno/extra/RenderPass.h>

namespace zeno {
ZENO_API RenderPassBase::RenderPassBase(std::string const &name)
    : name(name), refCount(0) 
    {
        static int id_{0};
        id = id_++;
    }

ZENO_API RenderPassBase::RenderPassBase() = default;

ZENO_API RenderPassBase::~RenderPassBase() = default;

ZENO_API GeoPass::GeoPass(): RenderPassBase() {}
ZENO_API GeoPass::GeoPass(const std::string &name)
    : RenderPassBase(name) {}

ZENO_API GeoPass::~GeoPass() = default;



ZENO_API size_t GeoPass::serializeSize() const {
    size_t size{0};

    auto nameLen{name.size()};
    size += sizeof(nameLen);
    size += nameLen;

    auto createsLen{creates.size()};
    size += sizeof(createsLen);
    for(auto &create : creates)
    {
        auto createStrSize = create->serializeSize();
        size += sizeof(createStrSize);
        size += createStrSize;
    }

    auto readsLen{reads.size()};
    size += sizeof(readsLen);
    for(auto &read : reads)
    {
        auto readStrSize = read->actualSerializeSize();
        size += sizeof(readStrSize);
        size += readStrSize;
    }

    auto writesLen{writes.size()};
    size += sizeof(writesLen);
    for(auto &write : writes)
    {
        auto writeStrSize = write->actualSerializeSize();
        size += sizeof(writeStrSize);
        size += writeStrSize;
    }

    size += sizeof(refCount);
    size += sizeof(id);

    return size;
}

ZENO_API std::vector<char> GeoPass::serialize() const {
    std::vector<char> str;
    str.resize(serializeSize());
    serialize(str.data());
    return str;
}

ZENO_API void GeoPass::serialize(char *str) const {
    size_t i{0};

    auto nameLen{name.size()};
    memcpy(str + i, &nameLen, sizeof(nameLen));
    i += sizeof(nameLen);
    name.copy(str + i, nameLen);
    i += nameLen;

    auto createsLen{creates.size()};
    memcpy(str + i, &createsLen, sizeof(createsLen));
    i += sizeof(createsLen);
    for(const auto &create : creates){
        auto createStr = create->serialize();
        auto createStrSize = createStr.size();
        memcpy(str + i, &createStrSize, sizeof(createStrSize));
        i += sizeof(createStrSize);
        memcpy(str + i, createStr.data(), createStrSize);
        i += createStrSize;
    }

    auto readsLen{reads.size()};
    memcpy(str + i, &readsLen, sizeof(readsLen));
    i += sizeof(readsLen);
    for(const auto &read : reads){
        auto readStr = read->serialize();
        auto readStrSize = readStr.size();
        memcpy(str + i, &readStrSize, sizeof(readStrSize));
        i += sizeof(readStrSize);
        memcpy(str + i, readStr.data(), readStrSize);
        i += readStrSize;
    }

    auto writesLen{writes.size()};
    memcpy(str + i, &writesLen, sizeof(writesLen));
    i += sizeof(writesLen);
    for(const auto &write : writes){
        auto writeStr = write->serialize();
        auto writeStrSize = writeStr.size();
        memcpy(str + i, &writeStrSize, sizeof(writeStrSize));
        i += sizeof(writeStrSize);
        memcpy(str + i, writeStr.data(), writeStrSize);
        i += writeStrSize;
    }

    memcpy(str + i, &refCount, sizeof(refCount));
    i += sizeof(refCount);

    memcpy(str + i, &id, sizeof(id));
    i += sizeof(id);
}

ZENO_API GeoPass GeoPass::deserialize(std::vector<char> const &str) {
    GeoPass renderPass;

    size_t i{0};

    size_t nameLen;
    memcpy(&nameLen, str.data() + i, sizeof(nameLen));
    i += sizeof(nameLen);
    renderPass.name = std::string{str.data() + i, nameLen};
    i += nameLen;

    size_t createsLen;
    memcpy(&createsLen, str.data() + i, sizeof(createsLen));
    i += sizeof(createsLen);
    renderPass.creates.resize(createsLen);
    for(size_t j{0}; j < createsLen; ++j){
        size_t createStrLen;
        memcpy(&createStrLen, str.data() + i, sizeof(createStrLen));
        i += sizeof(createStrLen);

        std::vector<char> createStr;
        createStr.resize(createStrLen);
        memcpy(createStr.data(), str.data() + i, createStrLen);
        i += createStrLen;

        auto create = std::make_shared<GeoResource>(GeoResource::deserialize(createStr));
        renderPass.creates[j] = create;
    }

    size_t readsLen;
    memcpy(&readsLen, str.data() + i, sizeof(readsLen));
    i += sizeof(readsLen);
    renderPass.reads.resize(readsLen);
    for(size_t j{0}; j < readsLen; ++j){
        size_t readStrLen;
        memcpy(&readStrLen, str.data() + i, sizeof(readStrLen));
        i += sizeof(readStrLen);

        std::vector<char> readStr;
        readStr.resize(readStrLen);
        memcpy(readStr.data(), str.data() + i, readStrLen);
        i += readStrLen;

        auto read = std::make_shared<GeoResource>(GeoResource::deserialize(readStr));
        renderPass.reads[j] = read;
    }

    size_t writesLen;
    memcpy(&writesLen, str.data() + i, sizeof(writesLen));
    i += sizeof(writesLen);
    renderPass.writes.resize(writesLen);
    for(size_t j{0}; j < writesLen; ++j){
        size_t writeStrLen;
        memcpy(&writeStrLen, str.data() + i, sizeof(writeStrLen));
        i += sizeof(writeStrLen);

        std::vector<char> writeStr;
        writeStr.resize(writeStrLen);
        memcpy(writeStr.data(), str.data() + i, writeStrLen);
        i += writeStrLen;

        auto write = std::make_shared<GeoResource>(GeoResource::deserialize(writeStr));
        renderPass.writes[j] = write;
    }

    memcpy(&renderPass.refCount, str.data() + i, sizeof(renderPass.refCount));
    i += sizeof(renderPass.refCount);

    memcpy(&renderPass.id, str.data() + i, sizeof(renderPass.id));
    i += sizeof(renderPass.id);

    return renderPass;
}

ZENO_API void GeoPass::render() {
    std::cout << "Render GeoPass " << name << std::endl;
}

}