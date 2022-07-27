#pragma once

#include <zeno/core/IObject.h>
#include <zeno/types/PrimitiveObject.h>
#include <zeno/types/MaterialObject.h>
#include <zeno/extra/RenderPass.h>
#include <zeno/funcs/ObjectCodec.h>
#include <vector>

namespace zeno {
struct ResourceBase : IObject{
    std::size_t type = -1;
    std::size_t id;
    std::string name;
    std::size_t refCount;

    int creator;
    std::vector<std::size_t> readers;
    std::vector<std::size_t> writers;

    ZENO_API ResourceBase();
    ZENO_API ResourceBase(std::string const &name, int const &creator);
    ZENO_API virtual ~ResourceBase();

    ZENO_API virtual size_t serializeSize() const {return 0;}
    ZENO_API virtual size_t actualSerializeSize() const {return 0;}
    ZENO_API virtual std::vector<char> serialize() const {return std::vector<char>();}
    ZENO_API virtual void serialize(char *str) const {}
    ZENO_API static ResourceBase deserialize(std::vector<char> const &str) {return ResourceBase("", 0);}

    ZENO_API virtual void instantiate() {};
    ZENO_API virtual void release() {std::cout << "base\n";};
};

struct GeoResourceDataType {
    std::shared_ptr<PrimitiveObject> prim;
    std::shared_ptr<MaterialObject> mtl;

    GeoResourceDataType(std::shared_ptr<PrimitiveObject> const &prim, std::shared_ptr<MaterialObject> const &mtl)
        : prim(prim), mtl(mtl) {}
    ~GeoResourceDataType() = default;

    size_t serializeSize() const {
        prim->mtl = mtl;
        std::vector<char> buf;
        encodeObject(prim.get(), buf);
        return buf.size();
    }

    void serialize(std::back_insert_iterator<std::vector<char>> it) const{
        prim->mtl = mtl;
        std::vector<char> buf;
        encodeObject(prim.get(), buf);
        for(char c : buf)
            *it++ = c;
    };

    static GeoResourceDataType deserialize(char const *str){
        auto prim = std::static_pointer_cast<PrimitiveObject>(decodeObject(str, 100));
        GeoResourceDataType data(prim, prim->mtl);
        return data;
    }

};

struct GeoResource : ResourceBase {
    std::size_t type = 0;
    GeoResourceDataType resourceData;

    ZENO_API GeoResource(std::string const &name, int const creator);
    ZENO_API GeoResource(std::string const &name, std::shared_ptr<PrimitiveObject> const &prim, std::shared_ptr<MaterialObject> const &mtl);
    ZENO_API GeoResource(std::string const &name);
    ZENO_API GeoResource();
    ZENO_API ~GeoResource();

    ZENO_API size_t serializeSize() const override;
    ZENO_API size_t actualSerializeSize() const override;
    ZENO_API std::vector<char> serialize() const override;
    ZENO_API void serialize(char *str) const override;
    ZENO_API static GeoResource deserialize(std::vector<char> const &str);

    ZENO_API virtual void instantiate() override;
    ZENO_API virtual void release() override;
};

struct TextureResourceDataType {
    std::size_t id;
    std::size_t width;
    std::size_t height;

    TextureResourceDataType(std::size_t const width, std::size_t const height)
        : width(width), height(height)
    {
        static int id_{0};
        id = id_++;
    }

    TextureResourceDataType() = default;
    ~TextureResourceDataType() = default;

    size_t serializeSize() const {
        size_t size{0};

        size += sizeof(id);
        size += sizeof(width);
        size += sizeof(height);

        return size;
    }

    std::vector<char> serialize() const{
        std::vector<char> str;
        str.resize(serializeSize());
        serialize(str.data());
        return str;
    };

    void serialize(char *str) const {
        size_t i{0};

        memcpy(str + i, &id, sizeof(id));
        i += sizeof(id);
        memcpy(str + i, &width, sizeof(width));
        i += sizeof(width);
        memcpy(str + i, &height, sizeof(height));
        i += sizeof(height);
    }

    static TextureResourceDataType deserialize(char const *str){
        TextureResourceDataType data;

        size_t i{0};

        memcpy(&data.id, str + i, sizeof(data.id));
        i += sizeof(data.id);
        memcpy(&data.width, str + i, sizeof(data.width));
        i += sizeof(data.width);
        memcpy(&data.height, str + i, sizeof(data.height));
        i += sizeof(data.height);

        return data;
    }

};

struct TextureResource : ResourceBase {
    std::size_t type = 1;
    TextureResourceDataType resourceData;

    ZENO_API TextureResource(std::string const &name, int creator, std::size_t const width, std::size_t const height);
    ZENO_API TextureResource(std::string const &name, std::size_t const width, std::size_t const height);
    ZENO_API TextureResource(std::string const &name, int creator);
    ZENO_API TextureResource(std::string const &name);
    ZENO_API TextureResource();
    ZENO_API ~TextureResource();

    ZENO_API size_t serializeSize() const override;
    ZENO_API size_t actualSerializeSize() const override;
    ZENO_API std::vector<char> serialize() const override;
    ZENO_API void serialize(char *str) const override;
    ZENO_API static TextureResource deserialize(std::vector<char> const &str);

    ZENO_API virtual void instantiate() override;
    ZENO_API virtual void release() override;
};



}