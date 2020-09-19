#include <parser/root.h>

#include <parser/type.h>
#include <parser/function.h>
#include <parser/variable.h>

RootNode::RootNode(State &value) : Node(value, Kind::Root) {
	while (!end()) {
	    push({
            link<FunctionNode>(),
            link<TypeNode>(),
            link<VariableNode>()
	    });

	    if (children.back()->is(Kind::Variable))
	        error("I'm too lazy to interpret global variables.");
	}
}
