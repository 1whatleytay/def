#include <parser/new.h>

#include <parser/reference.h>

NewNode::NewNode(Node *parent) : Node(parent, Kind::New) {
	needs("def", true);

	push<ReferenceNode>();
}
