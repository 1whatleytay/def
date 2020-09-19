#pragma once

#include <parser/kinds.h>

class ReturnNode : public Node {
public:
	explicit ReturnNode(Node *parent);
};
