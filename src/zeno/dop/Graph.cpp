#include <zeno/dop/Graph.h>
#include <zeno/dop/Node.h>
#include <zeno/dop/Descriptor.h>


ZENO_NAMESPACE_BEGIN
namespace dop {


Node *Graph::add_node(Descriptor const &desc) {
    auto node = desc.factory();
    node->inputs.resize(desc.inputs.size());
    node->outputs.resize(desc.outputs.size());
    auto p = node.get();
    nodes.emplace_back(std::move(node));
    return p;
}


}
ZENO_NAMESPACE_END
