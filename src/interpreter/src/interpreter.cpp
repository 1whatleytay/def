#include <interpreter/interpreter.h>

#include <parser/assign.h>
#include <parser/number.h>
#include <parser/variable.h>
#include <parser/reference.h>

#include <fmt/format.h>

Instance::Instance(double value) : value(value) { }
Instance::Instance(TypeNode *type, Frame &frame) : type(type) {
    for (const auto &child : type->children) {
        if (child->is(Kind::Variable)) {
            auto *e = child->as<VariableNode>();

            items[e] = frame.make(e->children.front()->as<ExpressionNode>());
            if (!items[e])
                throw VerifyError(e, "Cannot assign value of empty return type to variable.");
        }
    }
}

Node *Frame::scope(Node *request, const std::function<bool(Node *)> &checker) {
    Node *p = request->parent;

    while (p) {
        for (const auto &child : p->children) {
            if (checker(child.get())) {
                return child.get();
            }
        }

        p = p->parent;
    }

    return nullptr;
}

std::shared_ptr<Instance> Frame::make(ExpressionNoun *noun) {
    auto *content = noun->value.front();

    FunctionNode *selected = nullptr;

    std::shared_ptr<Instance> result;

    switch (content->is<Kind>()) {
        case Kind::Parentheses:
            result = make(content->children.front()->as<ExpressionNode>());
            break;

        case Kind::Number:
            result = std::make_shared<Instance>(content->as<NumberNode>()->value);
            break;

        case Kind::New: {
            std::string name = content->children.front()->as<ReferenceNode>()->name;
            auto *type = scope(content, [&name](Node *node) {
                return node->is(Kind::Type) && node->as<TypeNode>()->name == name;
            })->as<TypeNode>();

            if (!type)
                throw std::runtime_error(fmt::format("Could not find type {}.", name));

            result = std::make_shared<Instance>(type, *this);

            break;
        }

        case Kind::Reference: {
            std::string name = content->as<ReferenceNode>()->name;

            auto *function = scope(content, [&name](Node *node) {
                return node->is(Kind::Function) && node->as<FunctionNode>()->name == name;
            })->as<FunctionNode>();

            if (function) {
                selected = function;
            } else {
                auto local = locals.find(name);

                if (local == locals.end())
                    throw VerifyError(content, "Cannot find local {}.", name);

                result = local->second;
            }

            break;
        }
        default:
            throw VerifyError(content, "Undefined expression content kind.");
    }

    for (size_t a = 1; a < noun->value.size(); a++) {
        auto *node = noun->value[a];

        switch (node->is<Kind>()) {
            case Kind::Dot: {
                if (!result)
                    throw VerifyError(node, "Cannot access dot of empty function return type.");

                if (!result->type)
                    throw VerifyError(node, "Dot operator used on number type.");

                selected = nullptr;

                std::string name = node->children.front()->as<ReferenceNode>()->name;

                Node *value = nullptr;

                for (const auto &child : result->type->children) {
                    if (child->is(Kind::Variable) && child->as<VariableNode>()->name == name
                        || child->is(Kind::Function) && child->as<FunctionNode>()->name == name) {
                        value = child.get();
                        break;
                    }
                }

                if (!value) {
                    throw VerifyError(node, "Cannot find property {} on type {}.", name, result->type->name);
                }

                if (value->is(Kind::Variable)) {
                    auto item = result->items.find(value->as<VariableNode>());
                    if (item == result->items.end())
                        throw VerifyError(node, "Error accessing variable {} on type {}.", name, result->type->name);

                    result = item->second;
                } else if (value->is(Kind::Function)) {
                    selected = value->as<FunctionNode>();
                } else {
                    throw VerifyError(node, "Unknown selected type field.");
                }

                break;
            }

            case Kind::Call: {
                if (!selected)
                    throw VerifyError(node, "Expression is not callable.");

                std::vector<std::shared_ptr<Instance>> parameters;
                parameters.reserve(node->children.size());

                for (const auto &child : node->children)
                    parameters.push_back(make(child->as<ExpressionNode>()));

                if (selected->stub) {
                    // not even going to bother to set up a good calling system
                    if (selected->name == "print") {
                        if (parameters.front()->type)
                            throw VerifyError(node, "Parameter to call must be number.");

                        fmt::print("{}\n", parameters.front()->value);
                    }
                } else {
                    result = Frame(selected, parameters, result).returnValue;
                }

                break;
            }

            default:
                throw VerifyError(node, "Undefined expression modifier kind.");
        }
    }

    return result;
}

std::shared_ptr<Instance> Frame::make(ExpressionCombinator *combinator) {
    auto a = make(combinator->a.get());
    auto b = make(combinator->b.get());

    if (a->type || b->type)
        throw VerifyError(combinator->op, "Math operators must take two numbers as parameters, got {} and {}.",
                a->type ? a->type->name : "Number", b->type ? b->type->name : "Number");

    switch (combinator->op->op) {
        case OperatorNode::Operator::Add:
            return std::make_shared<Instance>(a->value + b->value);
        case OperatorNode::Operator::Sub:
            return std::make_shared<Instance>(a->value - b->value);
        case OperatorNode::Operator::Mul:
            return std::make_shared<Instance>(a->value * b->value);
        case OperatorNode::Operator::Div:
            return std::make_shared<Instance>(a->value / b->value);
        default:
            throw VerifyError(combinator->op, "Unknown combinator op type.");
    }
}

std::shared_ptr<Instance> Frame::make(ExpressionResult *result) {
    switch (result->format) {
        case ExpressionResult::ResultFormat::Noun:
            return make(reinterpret_cast<ExpressionNoun *>(result));
        case ExpressionResult::ResultFormat::Combinator:
            return make(reinterpret_cast<ExpressionCombinator *>(result));
        default:
            throw std::runtime_error("Unknown result format type.");
    }
}

std::shared_ptr<Instance> Frame::make(ExpressionNode *node) {
    auto result = node->result();

    return make(result.get());
}

Frame::Frame(FunctionNode *node,
    const std::vector<std::shared_ptr<Instance>> &parameters, const std::shared_ptr<Instance> &def) {
    for (size_t a = 0; a < node->parameters; a++) {
        auto *e = node->children[a]->as<VariableNode>();

        if (a >= parameters.size()) {
            locals[e->name] = make(e->children.front()->as<ExpressionNode>());

            if (!locals[e->name])
                throw VerifyError(e, "Cannot assign empty function return type.");
        } else {
            locals[e->name] = parameters[a];
        }
    }

    if (def)
        locals["def"] = def;

    for (size_t a = node->parameters; a < node->children.size(); a++) {
        const auto &current = node->children[a];

        switch (current->is<Kind>()) {
            case Kind::Variable: {
                auto *e = current->as<VariableNode>();

                if (locals.find(e->name) != locals.end())
                    throw VerifyError(current.get(), "Variable {} is already declared.", e->name);

                locals[e->name] = make(current->children.front()->as<ExpressionNode>());

                if (!locals[e->name])
                    throw VerifyError(current.get(), "Cannot access dot of empty function return type.");

                break;
            }

            case Kind::Assign: {
                auto *e = current->as<AssignNode>();

                // oh dear
                std::shared_ptr<Instance> *ref;

                std::string firstName = e->children.front()->as<ReferenceNode>()->name;
                if (locals.find(firstName) == locals.end())
                    throw VerifyError(e, "Cannot find variable {}.", firstName);

                ref = &locals.at(firstName); // im hoping this will work

                for (size_t b = 1; b < current->children.size() - 1; b++) {
                    auto *mod = current->children[b].get(); // DotNode

                    if (!(*ref)->type)
                        throw VerifyError(mod, "Cannot use dot operator on number.");

                    std::string name = mod->children.front()->as<ReferenceNode>()->name;

                    VariableNode *value = nullptr;

                    for (const auto &child : (*ref)->type->children) {
                        if (child->is(Kind::Variable) && child->as<VariableNode>()->name == name) {
                            value = child->as<VariableNode>();
                            break;
                        }
                    }

                    if (!value) {
                        throw VerifyError(mod, "Cannot find property {} on type {}.", name, (*ref)->type->name);
                    }

                    if ((*ref)->items.find(value->as<VariableNode>()) == (*ref)->items.end()) {
                        throw VerifyError(mod, "Error accessing variable {} on type {}.", name, (*ref)->type->name);
                    }

                    ref = &(*ref)->items.at(value->as<VariableNode>());
                }

                *ref = make(current->children.back()->as<ExpressionNode>());
                if (!*ref)
                    throw VerifyError(current.get(), "Cannot assign function of no return value.");

                break;
            }

            case Kind::Return:
                returnValue = make(current->children.front()->as<ExpressionNode>());

                return;

            case Kind::Expression:
                make(current->as<ExpressionNode>());
                break;

            default:
                throw VerifyError(current.get(), "Unknown function command type.");
        }
    }
}
