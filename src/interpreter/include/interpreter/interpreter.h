#pragma once

#include <interpreter/error.h>

#include <parser/type.h>
#include <parser/root.h>
#include <parser/function.h>
#include <parser/variable.h>
#include <parser/expression.h>

#include <memory>
#include <unordered_map>

class Frame;

class Instance {
public:
    TypeNode *type = nullptr;
    double value = 0;
    std::unordered_map<VariableNode *, std::shared_ptr<Instance>> items;

    explicit Instance(double value);
    explicit Instance(TypeNode *type, Frame &frame);
};

class Frame {
public:
    std::shared_ptr<Instance> returnValue;

    std::unordered_map<std::string, std::shared_ptr<Instance>> locals;

    static Node *scope(Node *request, const std::function<bool(Node *)> &checker);

    std::shared_ptr<Instance> make(ExpressionNoun *noun);
    std::shared_ptr<Instance> make(ExpressionCombinator *combinator);
    std::shared_ptr<Instance> make(ExpressionResult *result);
    std::shared_ptr<Instance> make(ExpressionNode *node);

    explicit Frame(FunctionNode *node,
        const std::vector<std::shared_ptr<Instance>> &parameters, const std::shared_ptr<Instance> &def = nullptr);
};
