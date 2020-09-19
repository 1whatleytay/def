#pragma once

#include <parser/kinds.h>

class FunctionNode : public Node {
public:
    std::string name;

    size_t parameters = 0;

    bool stub = false;

	explicit FunctionNode(Node *parent);
};
