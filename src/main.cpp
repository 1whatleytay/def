#include <parser/root.h>

#include <interpreter/interpreter.h>

#include <fmt/printf.h>

#include <fstream>
#include <sstream>

int main(int count, const char **args) {
    if (count != 2) {
        fmt::print("Usage: def <path/to/source.def>\n");
        return 0;
    }

    std::string source;

    {
        std::ifstream stream(args[1]);
        std::stringstream buffer;
        buffer << stream.rdbuf();

        source = buffer.str();
    }

    State state(source);

    try {
        RootNode root(state);

        auto function = std::find_if(root.children.begin(), root.children.end(), [](const std::unique_ptr<Node> &node) {
            return node->is(Kind::Function) && node->as<FunctionNode>()->name == "main";
        });

        if (function == root.children.end()) {
            fmt::print("Can't find main function.\n");
            return 0;
        }

        try {
            Frame frame((*function)->as<FunctionNode>(), {});
        } catch (const VerifyError &error) {
            LineDetails details(state.text, error.node->index, false);

            fmt::print("ERROR: {} [line {}]\n{}\n{}\n",
                error.issue, details.lineNumber, details.line, details.marker);
        }
    } catch (const ParseError &error) {
        LineDetails details(state.text, error.index);

        fmt::print("ERROR: {} [line {}]\n{}\n{}\n",
            error.issue, details.lineNumber, details.line, details.marker);
    }

    return 0;
}
