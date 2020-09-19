#pragma once

#include <parser/kinds.h>

class NewNode : public Node {
public:
	explicit NewNode(Node *parent);
};
