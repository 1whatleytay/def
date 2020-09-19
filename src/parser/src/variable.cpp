#include <parser/variable.h>

#include <parser/expression.h>

VariableNode::VariableNode(Node *parent) : Node(parent, Kind::Variable) {
	needs("def", true);

	name = token();

	match("=");

    push<ExpressionNode>();
}
