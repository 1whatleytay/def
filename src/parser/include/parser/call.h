#pragma once

#include <parser/kinds.h>

class CallNode : public Node {
public:
	explicit CallNode(Node *parent);
};
