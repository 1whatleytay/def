#pragma once

#include <parser/kinds.h>

class TypeNode : public Node {
public:
    std::string name;

	explicit TypeNode(Node *parent);
};
