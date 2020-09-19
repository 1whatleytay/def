#include <parser/number.h>

NumberNode::NumberNode(Node *parent) : Node(parent, Kind::Number) {
	std::string result = token();

	if (next(".")) {
	    result += "." + token();
	}

	try {
	    value = std::stod(result);
	} catch (const std::invalid_argument &err) {
        // no fmt, too lazy to include
	    error(std::string("Expected number but got ") + result + std::string("."));
	}
}
