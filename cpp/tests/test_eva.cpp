#include <iostream>
#include <cassert>
#include <any>
#include "../include/Eva.h"

void test_self_eval() {
    Eva eva;
    
    // Test numbers
    auto result = eva.evalGlobal("42");
    assert(std::any_cast<int>(result) == 42);
    
    // Test strings
    result = eva.evalGlobal("\"hello\"");
    assert(std::any_cast<std::string>(result) == "hello");
    
    std::cout << "Self-evaluation tests passed!" << std::endl;
}

void test_math() {
    Eva eva;
    
    // Addition
    auto result = eva.evalGlobal("(+ 1 5)");
    assert(std::any_cast<int>(result) == 6);
    
    // Nested operations
    result = eva.evalGlobal("(+ (+ 3 2) 5)");
    assert(std::any_cast<int>(result) == 10);
    
    // Subtraction
    result = eva.evalGlobal("(- 10 5)");
    assert(std::any_cast<int>(result) == 5);
    
    // Multiplication
    result = eva.evalGlobal("(* 2 5)");
    assert(std::any_cast<int>(result) == 10);
    
    // Division
    result = eva.evalGlobal("(/ 10 2)");
    assert(std::any_cast<int>(result) == 5);
    
    std::cout << "Math tests passed!" << std::endl;
}

void test_variables() {
    Eva eva;
    
    // Variable declaration
    eva.evalGlobal("(var x 10)");
    auto result = eva.evalGlobal("x");
    assert(std::any_cast<int>(result) == 10);
    
    // Variable assignment
    eva.evalGlobal("(set x 20)");
    result = eva.evalGlobal("x");
    assert(std::any_cast<int>(result) == 20);
    
    std::cout << "Variable tests passed!" << std::endl;
}

void test_blocks() {
    Eva eva;
    
    auto result = eva.evalGlobal("(begin (var x 10) (var y 20) (+ x y))");
    assert(std::any_cast<int>(result) == 30);
    
    std::cout << "Block tests passed!" << std::endl;
}

void test_if() {
    Eva eva;
    
    // True condition
    auto result = eva.evalGlobal("(if (> 5 3) 100 200)");
    assert(std::any_cast<int>(result) == 100);
    
    // False condition
    result = eva.evalGlobal("(if (< 5 3) 100 200)");
    assert(std::any_cast<int>(result) == 200);
    
    std::cout << "If tests passed!" << std::endl;
}

void test_lambda() {
    Eva eva;
    
    // Simple lambda
    eva.evalGlobal("(var square (lambda (x) (* x x)))");
    auto result = eva.evalGlobal("(square 5)");
    assert(std::any_cast<int>(result) == 25);
    
    // Lambda with multiple parameters
    eva.evalGlobal("(var add (lambda (x y) (+ x y)))");
    result = eva.evalGlobal("(add 3 7)");
    assert(std::any_cast<int>(result) == 10);
    
    std::cout << "Lambda tests passed!" << std::endl;
}

int main() {
    test_self_eval();
    test_math();
    test_variables();
    test_blocks();
    test_if();
    test_lambda();
    
    std::cout << "All Eva interpreter tests passed!" << std::endl;
    return 0;
}