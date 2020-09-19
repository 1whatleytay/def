#include <parser/call.h>

#include <parser/expression.h>

CallNode::CallNode(Node *parent) : Node(parent, Kind::Call) {
	match("(");

	while (!end() && !peek(")")) {
	    push<ExpressionNode>();

	    next(",");
	}

	needs(")");
}
