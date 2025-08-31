#include "Transformer.h"
#include <stdexcept>
#include <functional>

ASTNodePtr Transformer::makeSymbol(const std::string& name) {
    return std::make_shared<SymbolNode>(name);
}

ASTNodePtr Transformer::makeList(const std::vector<ASTNodePtr>& elements) {
    return std::make_shared<ListNode>(elements);
}

ASTNodePtr Transformer::transformDefToLambda(const std::vector<ASTNodePtr>& defExp) {
    // def name params body -> var name (lambda params body)
    if (defExp.size() != 4) {
        throw std::runtime_error("def expects 3 arguments");
    }
    
    auto name = defExp[1];
    auto params = defExp[2];
    auto body = defExp[3];
    
    // Create (lambda params body)
    std::vector<ASTNodePtr> lambdaElements = {
        makeSymbol("lambda"),
        params,
        body
    };
    auto lambda = makeList(lambdaElements);
    
    // Create (var name lambda)
    std::vector<ASTNodePtr> varElements = {
        makeSymbol("var"),
        name,
        lambda
    };
    return makeList(varElements);
}

ASTNodePtr Transformer::transformSwitchToIf(const std::vector<ASTNodePtr>& switchExp) {
    // switch (cond1 block1) (cond2 block2) ... (else blockN)
    if (switchExp.size() < 2) {
        throw std::runtime_error("switch expects at least 1 case");
    }
    
    std::vector<ASTNodePtr> cases(switchExp.begin() + 1, switchExp.end());
    
    // Build nested if expressions
    std::function<ASTNodePtr(size_t)> buildIf = [&](size_t index) -> ASTNodePtr {
        if (index >= cases.size()) {
            return makeSymbol("null"); // No more cases
        }
        
        auto case_node = std::dynamic_pointer_cast<ListNode>(cases[index]);
        if (!case_node || case_node->size() != 2) {
            throw std::runtime_error("Each switch case must have condition and block");
        }
        
        auto condition = case_node->getElements()[0];
        auto block = case_node->getElements()[1];
        
        // Check if this is the else case
        auto cond_sym = std::dynamic_pointer_cast<SymbolNode>(condition);
        if (cond_sym && cond_sym->getName() == "else") {
            return block; // Return the else block directly
        }
        
        // Create (if condition block next)
        auto next = buildIf(index + 1);
        std::vector<ASTNodePtr> ifElements = {
            makeSymbol("if"),
            condition,
            block,
            next
        };
        return makeList(ifElements);
    };
    
    return buildIf(0);
}

ASTNodePtr Transformer::transformForToWhile(const std::vector<ASTNodePtr>& forExp) {
    // for init condition modifier exp -> begin init (while condition (begin exp modifier))
    if (forExp.size() != 5) {
        throw std::runtime_error("for expects 4 arguments");
    }
    
    auto init = forExp[1];
    auto condition = forExp[2];
    auto modifier = forExp[3];
    auto exp = forExp[4];
    
    // Create (begin exp modifier)
    std::vector<ASTNodePtr> loopBodyElements = {
        makeSymbol("begin"),
        exp,
        modifier
    };
    auto loopBody = makeList(loopBodyElements);
    
    // Create (while condition loopBody)
    std::vector<ASTNodePtr> whileElements = {
        makeSymbol("while"),
        condition,
        loopBody
    };
    auto whileLoop = makeList(whileElements);
    
    // Create (begin init whileLoop)
    std::vector<ASTNodePtr> beginElements = {
        makeSymbol("begin"),
        init,
        whileLoop
    };
    return makeList(beginElements);
}

ASTNodePtr Transformer::transformIncToSet(const std::vector<ASTNodePtr>& incExp) {
    // (++ varName) -> (set varName (+ varName 1))
    if (incExp.size() != 2) {
        throw std::runtime_error("++ expects 1 argument");
    }
    
    auto varName = incExp[1];
    
    // Create (+ varName 1)
    std::vector<ASTNodePtr> incrementElements = {
        makeSymbol("+"),
        varName,
        std::make_shared<NumberNode>(1)
    };
    auto increment = makeList(incrementElements);
    
    // Create (set varName increment)
    std::vector<ASTNodePtr> setElements = {
        makeSymbol("set"),
        varName,
        increment
    };
    return makeList(setElements);
}

ASTNodePtr Transformer::transformIncValToSet(const std::vector<ASTNodePtr>& incExp) {
    // (+= varName value) -> (set varName (+ varName value))
    if (incExp.size() != 3) {
        throw std::runtime_error("+= expects 2 arguments");
    }
    
    auto varName = incExp[1];
    auto value = incExp[2];
    
    // Create (+ varName value)
    std::vector<ASTNodePtr> incrementElements = {
        makeSymbol("+"),
        varName,
        value
    };
    auto increment = makeList(incrementElements);
    
    // Create (set varName increment)
    std::vector<ASTNodePtr> setElements = {
        makeSymbol("set"),
        varName,
        increment
    };
    return makeList(setElements);
}

ASTNodePtr Transformer::transformDecToSet(const std::vector<ASTNodePtr>& decExp) {
    // (-- varName) -> (set varName (- varName 1))
    if (decExp.size() != 2) {
        throw std::runtime_error("-- expects 1 argument");
    }
    
    auto varName = decExp[1];
    
    // Create (- varName 1)
    std::vector<ASTNodePtr> decrementElements = {
        makeSymbol("-"),
        varName,
        std::make_shared<NumberNode>(1)
    };
    auto decrement = makeList(decrementElements);
    
    // Create (set varName decrement)
    std::vector<ASTNodePtr> setElements = {
        makeSymbol("set"),
        varName,
        decrement
    };
    return makeList(setElements);
}

ASTNodePtr Transformer::transformDecValToSet(const std::vector<ASTNodePtr>& decExp) {
    // (-= varName value) -> (set varName (- varName value))
    if (decExp.size() != 3) {
        throw std::runtime_error("-= expects 2 arguments");
    }
    
    auto varName = decExp[1];
    auto value = decExp[2];
    
    // Create (- varName value)
    std::vector<ASTNodePtr> decrementElements = {
        makeSymbol("-"),
        varName,
        value
    };
    auto decrement = makeList(decrementElements);
    
    // Create (set varName decrement)
    std::vector<ASTNodePtr> setElements = {
        makeSymbol("set"),
        varName,
        decrement
    };
    return makeList(setElements);
}