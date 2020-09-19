#pragma once

#include <parser/kinds.h>

class ParenthesesNode : public Node {
public:
	explicit ParenthesesNode(Node *parent);
};
