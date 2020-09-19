#include <parser/assign.h>

#include <parser/dot.h>
#include <parser/reference.h>
#include <parser/expression.h>

AssignNode::AssignNode(Node *parent) : Node(parent, Kind::Assign) {
    // Sorry, I'm not making references and stuff like in pagent2, going to make this manual
	push<ReferenceNode>();

	while (push<DotNode>(true));

	match("=");

	push<ExpressionNode>();
}
