#pragma once

#include <parser/kinds.h>

#include <parser/operator.h>

class ExpressionResult {
public:
    enum class ResultFormat {
        Noun,
        Combinator
    };

    ResultFormat format = ResultFormat::Noun;

    explicit ExpressionResult(ResultFormat format);
    virtual ~ExpressionResult() = default;
};

class ExpressionNoun : public ExpressionResult {
public:
    std::vector<Node *> value;

    ExpressionNoun();
};

class ExpressionCombinator : public ExpressionResult {
public:
    std::unique_ptr<ExpressionResult> a;
    std::unique_ptr<ExpressionResult> b;

    OperatorNode *op = nullptr;

    ExpressionCombinator(ExpressionResult *a, ExpressionResult *b, OperatorNode *op);
};

class ExpressionNode : public Node {
public:
    std::unique_ptr<ExpressionResult> result();

	explicit ExpressionNode(Node *parent);
};
