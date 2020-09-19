#include <parser/function.h>

#include <parser/assign.h>
#include <parser/return.h>
#include <parser/variable.h>
#include <parser/expression.h>

FunctionNode::FunctionNode(Node *parent) : Node(parent, Kind::Function) {
	needs("def", true);

	name = token();

	match("(");

	while (!end() && !peek(")")) {
	    push<VariableNode>();
	    parameters++;

	    next(",");
	}

	needs(")");

	if (next("{")) {
        while (!end() && !peek("}")) {
            push({
                link<VariableNode>(),
                link<AssignNode>(),
                link<ReturnNode>(),
                link<ExpressionNode>()
            });
        }

        needs("}");
    } else {
	    stub = true;
	}
}
