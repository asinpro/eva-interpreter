#include <iostream>
#include <cassert>
#include <any>
#include "../include/Eva.h"

void test_def() {
    Eva eva;
    
    // Test function definition
    eva.evalGlobal("(def square (x) (* x x))");
    auto result = eva.evalGlobal("(square 4)");
    assert(std::any_cast<int>(result) == 16);
    
    // Test function with multiple parameters
    eva.evalGlobal("(def add (x y) (+ x y))");
    result = eva.evalGlobal("(add 3 7)");
    assert(std::any_cast<int>(result) == 10);
    
    std::cout << "Def transformation tests passed!" << std::endl;
}

void test_for_loop() {
    Eva eva;
    
    // Test for loop
    eva.evalGlobal("(var result 0)");
    eva.evalGlobal("(for (var i 1) (<= i 5) (++ i) (+= result i))");
    auto result = eva.evalGlobal("result");
    assert(std::any_cast<int>(result) == 15); // 1+2+3+4+5
    
    std::cout << "For loop transformation tests passed!" << std::endl;
}

void test_inc_dec() {
    Eva eva;
    
    // Test increment
    eva.evalGlobal("(var x 10)");
    eva.evalGlobal("(++ x)");
    auto result = eva.evalGlobal("x");
    assert(std::any_cast<int>(result) == 11);
    
    // Test increment with value
    eva.evalGlobal("(+= x 5)");
    result = eva.evalGlobal("x");
    assert(std::any_cast<int>(result) == 16);
    
    // Test decrement
    eva.evalGlobal("(-- x)");
    result = eva.evalGlobal("x");
    assert(std::any_cast<int>(result) == 15);
    
    // Test decrement with value
    eva.evalGlobal("(-= x 3)");
    result = eva.evalGlobal("x");
    assert(std::any_cast<int>(result) == 12);
    
    std::cout << "Increment/decrement transformation tests passed!" << std::endl;
}

void test_switch() {
    Eva eva;
    
    // Test switch statement
    eva.evalGlobal("(var x 2)");
    auto result = eva.evalGlobal(R"(
        (switch 
            ((== x 1) 100)
            ((== x 2) 200)
            (else 300))
    )");
    assert(std::any_cast<int>(result) == 200);
    
    // Test else case
    eva.evalGlobal("(set x 99)");
    result = eva.evalGlobal(R"(
        (switch 
            ((== x 1) 100)
            ((== x 2) 200)
            (else 300))
    )");
    assert(std::any_cast<int>(result) == 300);
    
    std::cout << "Switch transformation tests passed!" << std::endl;
}

int main() {
    test_def();
    test_inc_dec();
    test_for_loop();
    test_switch();
    
    std::cout << "All transformer tests passed!" << std::endl;
    return 0;
}