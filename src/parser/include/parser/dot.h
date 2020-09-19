#pragma once

#include <parser/kinds.h>

class DotNode : public Node {
public:
	explicit DotNode(Node *parent);
};
