#include <iostream>
#include <cassert>
#include <vector>
#include <any>
#include "../include/Parser.h"
#include "../include/AST.h"

void test_parser() {
    // Test number parsing
    auto node = EvaParser::parse("42");
    assert(node != nullptr);
    auto num_node = std::dynamic_pointer_cast<NumberNode>(node);
    assert(num_node != nullptr);
    assert(num_node->getValue() == 42);
    
    // Test string parsing
    node = EvaParser::parse("\"hello\"");
    assert(node != nullptr);
    auto str_node = std::dynamic_pointer_cast<StringNode>(node);
    assert(str_node != nullptr);
    assert(std::any_cast<std::string>(str_node->toValue()) == "hello");
    
    // Test symbol parsing
    node = EvaParser::parse("x");
    assert(node != nullptr);
    auto sym_node = std::dynamic_pointer_cast<SymbolNode>(node);
    assert(sym_node != nullptr);
    assert(sym_node->getName() == "x");
    
    // Test list parsing
    node = EvaParser::parse("(+ 1 2)");
    assert(node != nullptr);
    auto list_node = std::dynamic_pointer_cast<ListNode>(node);
    assert(list_node != nullptr);
    assert(list_node->size() == 3);
    
    // Test nested list
    node = EvaParser::parse("(+ (* 2 3) 4)");
    assert(node != nullptr);
    list_node = std::dynamic_pointer_cast<ListNode>(node);
    assert(list_node != nullptr);
    assert(list_node->size() == 3);
    
    std::cout << "Parser tests passed!" << std::endl;
}

int main() {
    test_parser();
    return 0;
}