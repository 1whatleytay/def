#pragma once

#include <parser/kinds.h>

class VariableNode : public Node {
public:
    std::string name;

	explicit VariableNode(Node *parent);
};
