#pragma once

#include <hermes/node.h>

#include <fmt/format.h>

#include <string>
#include <exception>

class VerifyError : std::exception {
public:
    std::string issue;
    hermes::Node *node = nullptr;

    [[nodiscard]] const char* what() const noexcept override;

    template <typename ...Args>
    VerifyError(hermes::Node *node, const char *format, Args ...args)
        : node(node), issue(fmt::format(format, args...)) { }
};
