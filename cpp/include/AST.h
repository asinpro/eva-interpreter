#pragma once

#include <memory>
#include <vector>
#include <string>
#include <any>

/**
 * Base class for AST nodes
 */
class ASTNode {
public:
    virtual ~ASTNode() = default;
    virtual std::any toValue() const = 0;
};

using ASTNodePtr = std::shared_ptr<ASTNode>;

/**
 * Number literal node
 */
class NumberNode : public ASTNode {
public:
    explicit NumberNode(int value) : value_(value) {}
    
    std::any toValue() const override {
        return value_;
    }
    
    int getValue() const { return value_; }

private:
    int value_;
};

/**
 * String literal node
 */
class StringNode : public ASTNode {
public:
    explicit StringNode(const std::string& value) : value_(value) {}
    
    std::any toValue() const override {
        // Remove quotes from string literal
        if (value_.length() >= 2 && value_[0] == '"' && value_.back() == '"') {
            return value_.substr(1, value_.length() - 2);
        }
        return value_;
    }
    
    const std::string& getValue() const { return value_; }

private:
    std::string value_;
};

/**
 * Symbol node
 */
class SymbolNode : public ASTNode {
public:
    explicit SymbolNode(const std::string& name) : name_(name) {}
    
    std::any toValue() const override {
        return name_;
    }
    
    const std::string& getName() const { return name_; }

private:
    std::string name_;
};

/**
 * List node (S-expression)
 */
class ListNode : public ASTNode {
public:
    explicit ListNode(const std::vector<ASTNodePtr>& elements) : elements_(elements) {}
    
    std::any toValue() const override {
        std::vector<std::any> result;
        for (const auto& element : elements_) {
            result.push_back(element->toValue());
        }
        return result;
    }
    
    const std::vector<ASTNodePtr>& getElements() const { return elements_; }
    
    size_t size() const { return elements_.size(); }
    
    bool empty() const { return elements_.empty(); }

private:
    std::vector<ASTNodePtr> elements_;
};

// Global variable to store parse result
extern ASTNodePtr parseResult;