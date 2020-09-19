#pragma once

#include <parser/kinds.h>

class ReferenceNode : public Node {
public:
    std::string name;

	explicit ReferenceNode(Node *parent);
};
