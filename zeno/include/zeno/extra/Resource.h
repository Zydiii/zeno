#pragma once

#include <zeno/core/IObject.h>
#include <zeno/types/PrimitiveObject.h>
#include <zeno/types/MaterialObject.h>
#include <zeno/extra/RenderPass.h>
#include <vector>
#include <zeno/funcs/ObjectCodec.h>

namespace zeno {
struct ResourceBase : IObject{
    std::size_t type = -1;
    std::size_t id;
    std::string name;
    std::size_t refCount;

    std::size_t creator;
    std::vector<std::size_t> readers;
    std::vector<std::size_t> writers;

    ZENO_API ResourceBase();
    ZENO_API ResourceBase(std::string const &name, std::size_t const &creator);
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

    static GeoResourceDataType deserialize(const char *str){
        auto prim = std::static_pointer_cast<PrimitiveObject>(decodeObject(str, 100));
        GeoResourceDataType data(prim, prim->mtl);
        return data;
    }

};

struct GeoResource : ResourceBase {
    std::size_t type = 0;
    GeoResourceDataType resourceData;

    ZENO_API GeoResource(std::string const &name, std::size_t const creator);
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



}