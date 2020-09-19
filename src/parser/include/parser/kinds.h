#pragma once

#include <hermes/node.h>

using namespace hermes;

enum class Kind {
    Root,
    Function,
    Variable,
    Expression,
    Operator,
    Type,
    Number,
    Reference,
    Parentheses,
    Dot,
    Call,
    New,
    Assign,
    Return,
};
