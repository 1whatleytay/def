#include <parser/return.h>

#include <parser/expression.h>

ReturnNode::ReturnNode(Node *parent) : Node(parent, Kind::Return) {
	needs("def", true);

	push<ExpressionNode>();
}
