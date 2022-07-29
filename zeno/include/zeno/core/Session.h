#pragma once

#include <zeno/utils/api.h>
#include <zeno/core/Descriptor.h>
#include <memory>
#include <string>
#include <map>

namespace zeno {

struct Graph;
struct Session;
struct INode;

struct INodeClass {
    std::unique_ptr<Descriptor> desc;

    ZENO_API INodeClass(Descriptor const &desc);
    ZENO_API virtual ~INodeClass();

    virtual std::unique_ptr<INode> new_instance() const = 0;
};

struct IObject;
struct GlobalState;
struct GlobalComm;
struct GlobalStatus;
struct EventCallbacks;

struct Session {
    std::map<std::string, std::unique_ptr<INodeClass>> nodeClasses;

    std::unique_ptr<GlobalState> const globalState;
    std::unique_ptr<GlobalComm> const globalComm;
    std::unique_ptr<GlobalStatus> const globalStatus;
    std::unique_ptr<EventCallbacks> const eventCallbacks;

    ZENO_API Session();
    ZENO_API ~Session();

    Session(Session const &) = delete;
    Session &operator=(Session const &) = delete;
    Session(Session &&) = delete;
    Session &operator=(Session &&) = delete;

    ZENO_API std::unique_ptr<Graph> createGraph();
    ZENO_API std::string dumpDescriptors() const;
    ZENO_API void defNodeClass(std::unique_ptr<INode>(*ctor)(), std::string const &id, Descriptor const &desc = {});
    //ZENO_API void defNodeClass(std::string const &id, std::unique_ptr<INodeClass> cls);
    //ZENO_API void defOverloadNodeClass(std::string const &id, std::vector<std::string> const &types,
            //std::unique_ptr<INodeClass> &&cls);
    //ZENO_API std::unique_ptr<INode> getOverloadNode(
            //std::string const &name, std::vector<std::shared_ptr<IObject>> const &inputs);
    //ZENO_API std::map<std::string, zany> callTempNode(std::string const &id,
            //std::map<std::string, zany> const &inputs) const;

    //void defNodeClass(std::unique_ptr<INode>(*ctor)(), std::string const &id, Descriptor const &desc = {}) {
        //defNodeClass(id, std::make_unique<ImplNodeClass<std::unique_ptr<INode>(*)()>>(ctor, desc));
    //}

    //template <class F>
    //void defOverloadNodeClass(F const &ctor, std::string const &id,
            //std::vector<std::string> const &types, Descriptor const &desc = {}) {
        //defOverloadNodeClass(id, types, std::make_unique<ImplNodeClass<F>>(ctor, desc));
    //}
};

ZENO_API Session &getSession();

}
