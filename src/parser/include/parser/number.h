#pragma once

#include <parser/kinds.h>

class NumberNode : public Node {
public:
    double value = 0;

	explicit NumberNode(Node *parent);
};
