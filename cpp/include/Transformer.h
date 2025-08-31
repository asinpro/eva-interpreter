#pragma once

#include "AST.h"
#include <memory>

/**
 * AST Transformer for syntactic sugar
 */
class Transformer {
public:
    /**
     * Translates a 'def' expression into variable declaration with lambda
     * def square (x) (* x x) -> var square (lambda (x) (* x x))
     */
    static ASTNodePtr transformDefToLambda(const std::vector<ASTNodePtr>& defExp);

    /**
     * Translates a 'switch' statement into nested if statement
     */
    static ASTNodePtr transformSwitchToIf(const std::vector<ASTNodePtr>& switchExp);

    /**
     * Translates 'for loop' into while loop
     */
    static ASTNodePtr transformForToWhile(const std::vector<ASTNodePtr>& forExp);

    /**
     * Translates increment to set operation
     */
    static ASTNodePtr transformIncToSet(const std::vector<ASTNodePtr>& incExp);

    /**
     * Translates increment with value to set operation
     */
    static ASTNodePtr transformIncValToSet(const std::vector<ASTNodePtr>& incExp);

    /**
     * Translates decrement to set operation
     */
    static ASTNodePtr transformDecToSet(const std::vector<ASTNodePtr>& decExp);

    /**
     * Translates decrement with value to set operation
     */
    static ASTNodePtr transformDecValToSet(const std::vector<ASTNodePtr>& decExp);

private:
    // Helper methods for creating AST nodes
    static ASTNodePtr makeSymbol(const std::string& name);
    static ASTNodePtr makeList(const std::vector<ASTNodePtr>& elements);
};