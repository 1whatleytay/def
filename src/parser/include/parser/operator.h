#pragma once

#include <parser/kinds.h>

class OperatorNode : public Node {
public:
    enum class Operator {
        Add,
        Sub,
        Mul,
        Div,
    };

    std::vector<std::string> names = {
        "+", // Add
        "-", // Sub
        "*", // Mul
        "/", // Div
    };

    Operator op = Operator::Add;

	explicit OperatorNode(Node *parent);
};
