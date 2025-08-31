#pragma once

#include <memory>
#include <any>
#include <functional>
#include "Environment.h"
#include "AST.h"
#include "Transformer.h"

/**
 * Eva interpreter main class
 */
class Eva {
public:
    using Value = std::any;
    using NativeFunction = std::function<Value(const std::vector<Value>&)>;

    /**
     * Creates an Eva instance with global environment
     */
    Eva();
    explicit Eva(std::shared_ptr<Environment> global);

    /**
     * Evaluates global code wrapping into a block
     */
    Value evalGlobal(const std::string& code);
    Value evalGlobal(ASTNodePtr exp);

    /**
     * Evaluates an expression in a given environment
     */
    Value eval(ASTNodePtr exp, std::shared_ptr<Environment> env = nullptr);

private:
    std::shared_ptr<Environment> global_;

    // Helper methods
    Value evalBlock(const std::vector<ASTNodePtr>& expressions, std::shared_ptr<Environment> env);
    Value callUserDefinedFunction(const Value& fn, const std::vector<Value>& args);
    bool isNumber(const Value& value);
    bool isString(const Value& value);
    bool isVariableName(const std::string& exp);
    
    // Built-in functions
    void setupGlobalEnvironment();
    static std::shared_ptr<Environment> createGlobalEnvironment();

    // Helper functions for creating AST nodes
    static ASTNodePtr makeSymbol(const std::string& name);
    static ASTNodePtr makeList(const std::vector<ASTNodePtr>& elements);

    // User function representation
    struct UserFunction {
        std::vector<std::string> params;
        ASTNodePtr body;
        std::shared_ptr<Environment> env; // closure
    };
};