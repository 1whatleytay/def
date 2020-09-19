#include <parser/expression.h>

#include <parser/new.h>
#include <parser/dot.h>
#include <parser/call.h>
#include <parser/number.h>
#include <parser/operator.h>
#include <parser/reference.h>
#include <parser/parentheses.h>


ExpressionResult::ExpressionResult(ResultFormat format) : format(format) { }

ExpressionNoun::ExpressionNoun() : ExpressionResult(ResultFormat::Noun) { }

ExpressionCombinator::ExpressionCombinator(ExpressionResult *a, ExpressionResult *b, OperatorNode *op)
    : ExpressionResult(ResultFormat::Combinator), a(a), b(b), op(op) { }


std::unique_ptr<ExpressionResult> ExpressionNode::result() {
    std::vector<std::unique_ptr<ExpressionResult>> results;
    std::vector<OperatorNode *> operators;

    std::unique_ptr<ExpressionNoun> current = std::make_unique<ExpressionNoun>();

    for (const auto &child : children) {
        if (child->is(Kind::Operator)) {
            results.push_back(std::move(current));
            operators.push_back(child->as<OperatorNode>());

            current = std::make_unique<ExpressionNoun>();
        } else {
            current->value.push_back(child.get());
        }
    }

    if (current->value.empty())
        throw std::runtime_error("Internal expression noun issue occurred.");

    results.emplace_back(std::move(current));

    std::vector<OperatorNode::Operator> operatorOrder = {
        OperatorNode::Operator::Mul,
        OperatorNode::Operator::Div,
        OperatorNode::Operator::Add,
        OperatorNode::Operator::Sub
    };

    while (!operators.empty()) {
        for (auto order : operatorOrder) {
            for (size_t i = 0; i < operators.size(); i++) {
                OperatorNode *op = operators[i];

                if (op->op != order)
                    continue;

                ExpressionResult *a = results[i].release();
                ExpressionResult *b = results[i + 1].release();

                results.erase(results.begin() + i, results.begin() + i + 2);
                operators.erase(operators.begin() + i);

                results.insert(results.begin() + i,
                    std::make_unique<ExpressionCombinator>(a, b, op));
            }
        }
    }

    if (results.size() != 1)
        throw std::runtime_error("Internal result picker issue occurred.");

    return std::move(results.front());
}

ExpressionNode::ExpressionNode(Node *parent) : Node(parent, Kind::Expression) {
    bool exit = false;

    while(!end() && !exit) {
        push({
            link<ParenthesesNode>(),
            link<NumberNode>(),
            link<NewNode>(),
            link<ReferenceNode>(),
        });

        while (true) {
            if (!push({
                link<DotNode>(),
                link<CallNode>(),
                link<OperatorNode>(),
            }, true)) {
                exit = true;
                break;
            }

            if (children.back()->is(Kind::Operator))
                break;
        }
    }
}
