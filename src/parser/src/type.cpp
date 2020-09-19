#include <parser/type.h>

#include <parser/function.h>
#include <parser/variable.h>

TypeNode::TypeNode(Node *parent) : Node(parent, Kind::Type) {
	needs("def", true);

	name = token();

	match("{");

	while (!end() && !peek("}")) {
	    push({
	        link<FunctionNode>(),
            link<VariableNode>()
	    });
	}

	needs("}");
}
