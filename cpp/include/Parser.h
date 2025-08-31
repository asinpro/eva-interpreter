#pragma once

#include <string>
#include <memory>
#include "AST.h"

/**
 * Parser interface for Eva expressions
 */
class EvaParser {
public:
    /**
     * Parse a string into an AST
     */
    static ASTNodePtr parse(const std::string& input);
};