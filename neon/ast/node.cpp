#include "node.h"

#define INDENT_MULTIPLIER 4

void Node::dump(const int indent) const {
    constexpr char * array[5] = {
        (char *)ColorCyan,
        (char *)ColorYellow,
        (char *)ColorBlue,
        (char *)ColorGreen,
        (char *)ColorMagenta,
    };

    std::cout << ColorGray << std::string(indent * INDENT_MULTIPLIER, ' ') << BoldFont << array[indent % (std::end(array) - std::begin(array))] << id << " " << ColorReset;

    if (data)
        std::cout << "\"" << data.value() << "\" ";

    if (position)
        std::cout << "<" << std::get<0>(position.value()) << ":" << std::get<1>(position.value()) << "> ";

    if (nodes.size() > 0) {
        std::cout << "{" << std::endl;

        for (Node child : nodes)
            child.dump(indent + 1);

        std::cout << std::string(indent * INDENT_MULTIPLIER, ' ') << "}" << std::endl;
    } else {
        std::cout << std::endl;
    }
}

std::optional<Node *> Node::get_node(AstId id) {
    auto it = std::find_if(std::begin(nodes), std::end(nodes), [id](const Node & node) {
        return node.id == id;
    });

    if (it == std::end(nodes))
        return std::nullopt;

    return &*it;
}

Node * Node::get_last_node() {
    if (nodes.empty())
        return this;

    return nodes.back().get_last_node();
}
