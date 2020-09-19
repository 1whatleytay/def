#pragma once

#include <parser/kinds.h>

class AssignNode : public Node {
public:
	explicit AssignNode(Node *parent);
};
