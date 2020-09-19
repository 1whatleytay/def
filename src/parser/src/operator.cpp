#include <parser/operator.h>

OperatorNode::OperatorNode(Node *parent) : Node(parent, Kind::Operator) {
	op = select<Operator>(names);
}
