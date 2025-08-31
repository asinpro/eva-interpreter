#include "Parser.h"
#include <sstream>
#include <cctype>
#include <stdexcept>

class SimpleParser {
private:
    std::string input_;
    size_t pos_;
    
    void skipWhitespace() {
        while (pos_ < input_.length() && std::isspace(input_[pos_])) {
            pos_++;
        }
    }
    
    char peek() {
        skipWhitespace();
        if (pos_ >= input_.length()) return '\0';
        return input_[pos_];
    }
    
    char consume() {
        skipWhitespace();
        if (pos_ >= input_.length()) return '\0';
        return input_[pos_++];
    }
    
    std::string parseString() {
        if (consume() != '"') throw std::runtime_error("Expected '\"'");
        std::string result = "\"";
        while (pos_ < input_.length() && input_[pos_] != '"') {
            result += input_[pos_++];
        }
        if (pos_ >= input_.length()) throw std::runtime_error("Unterminated string");
        consume(); // consume closing quote
        result += "\"";
        return result;
    }
    
    std::string parseSymbolOrNumber() {
        std::string result;
        while (pos_ < input_.length() && 
               (std::isalnum(input_[pos_]) || 
                input_[pos_] == '-' || input_[pos_] == '+' || 
                input_[pos_] == '*' || input_[pos_] == '/' ||
                input_[pos_] == '<' || input_[pos_] == '>' ||
                input_[pos_] == '=' || input_[pos_] == '%' ||
                input_[pos_] == '_')) {
            result += input_[pos_++];
        }
        return result;
    }
    
    bool isNumber(const std::string& str) {
        if (str.empty()) return false;
        size_t start = 0;
        if (str[0] == '-' || str[0] == '+') start = 1;
        for (size_t i = start; i < str.length(); i++) {
            if (!std::isdigit(str[i])) return false;
        }
        return start < str.length();
    }
    
public:
    SimpleParser(const std::string& input) : input_(input), pos_(0) {}
    
    ASTNodePtr parseExpression() {
        char c = peek();
        if (c == '\0') return nullptr;
        
        if (c == '(') {
            return parseList();
        } else if (c == '"') {
            std::string str = parseString();
            return std::make_shared<StringNode>(str);
        } else {
            std::string token = parseSymbolOrNumber();
            if (isNumber(token)) {
                return std::make_shared<NumberNode>(std::stoi(token));
            } else {
                return std::make_shared<SymbolNode>(token);
            }
        }
    }
    
    ASTNodePtr parseList() {
        if (consume() != '(') throw std::runtime_error("Expected '('");
        
        std::vector<ASTNodePtr> elements;
        while (peek() != ')' && peek() != '\0') {
            auto element = parseExpression();
            if (element) {
                elements.push_back(element);
            }
        }
        
        if (consume() != ')') throw std::runtime_error("Expected ')'");
        
        return std::make_shared<ListNode>(elements);
    }
};

ASTNodePtr EvaParser::parse(const std::string& input) {
    SimpleParser parser(input);
    return parser.parseExpression();
}