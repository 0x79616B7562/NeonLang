#pragma once

#include "node.h"
#include <neonc.h>
#include "operator.h"
#include "number.h"
#include "identifier.h"
#include "call.h"

namespace neonc {
    struct Expression : public Node {
        Expression(): Node(std::nullopt) {}

        virtual void dump(const uint32_t indentation) const {
            std::cout << ColorGreen << BoldFont << "( " << ColorReset;

            for (auto & n : nodes)
                n->dump(indentation);

            std::cout << ColorGreen << BoldFont << " )" << ColorReset;
        }

        llvm::Value * build(Module & module, llvm::Type * type) {
            llvm::Value * value = nullptr;
            std::optional<std::shared_ptr<Operator>> op = std::nullopt;

            for (auto & n : nodes) {
                auto expr = std::dynamic_pointer_cast<Expression>(n);

                if (expr) {
                    auto _value = expr->build(module, type);
                    value = op.has_value() ? op->get()->build(module, value, _value, type) : _value;

                    continue;
                }

                auto _op = std::dynamic_pointer_cast<Operator>(n);

                if (_op) {
                    op = _op;

                    continue;
                }

                auto num = std::dynamic_pointer_cast<Number>(n);

                if (num) {
                    auto _value = num->build(module, type);
                    value = op.has_value() ? op->get()->build(module, value, _value, type) : _value;
                    
                    continue;
                }

                auto identifier = std::dynamic_pointer_cast<Identifier>(n);

                if (identifier) {
                    if (module.local_variables.contains(identifier->identifier)) {
                        auto _value = module.get_builder()->CreateLoad(type, module.local_variables[identifier->identifier]);
                        value = op.has_value() ? op->get()->build(module, value, _value, type) : _value;
                    } else if (module.get_arguments().contains(identifier->identifier)) {
                        auto _value = module.get_arguments()[identifier->identifier];
                        value = op.has_value() ? op->get()->build(module, value, _value, type) : _value;
                    } else {
                        throw std::invalid_argument("ICE: unknown identifier");
                    }

                    continue;
                }

                auto call = std::dynamic_pointer_cast<Call>(n);

                if (call) {
                    std::vector<llvm::Value *> args;

                    for (uint32_t i = 0; i < call->nodes.size(); i++) {
                        auto expr = std::dynamic_pointer_cast<Expression>(call->nodes[i]);

                        if (expr) {
                            args.push_back(expr->build(module, module.module->getFunction(call->identifier)->getArg(i)->getType()));
                        }
                    }

                    auto _value = (llvm::Value *)call->build(module, args);
                    value = op.has_value() ? op->get()->build(module, value, _value, type) : _value;

                    continue;
                }
            }

            return value;
        }
    };
}