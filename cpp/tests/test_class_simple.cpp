#include <iostream>
#include <cassert>
#include <any>
#include "../include/Eva.h"

void test_simple_class() {
    Eva eva;
    
    std::cout << "Testing basic class definition..." << std::endl;
    
    // Step by step testing
    eva.evalGlobal("(class Point null (var x 10))");
    std::cout << "Class defined successfully" << std::endl;
    
    eva.evalGlobal("(var p (new Point))");
    std::cout << "Instance created successfully" << std::endl;
    
    auto result = eva.evalGlobal("(prop p x)");
    assert(std::any_cast<int>(result) == 10);
    std::cout << "Property access successful: " << std::any_cast<int>(result) << std::endl;
    
    std::cout << "Simple class tests passed!" << std::endl;
}

void test_class_with_constructor() {
    Eva eva;
    
    std::cout << "Testing class with constructor..." << std::endl;
    
    // Define a simple class with constructor
    eva.evalGlobal("(class Point null (def constructor (this x y) (begin (set (prop this x) x) (set (prop this y) y))))");
    std::cout << "Class with constructor defined" << std::endl;
    
    eva.evalGlobal("(var p (new Point 10 20))");
    std::cout << "Instance created with constructor args" << std::endl;
    
    auto x = eva.evalGlobal("(prop p x)");
    auto y = eva.evalGlobal("(prop p y)");
    
    assert(std::any_cast<int>(x) == 10);
    assert(std::any_cast<int>(y) == 20);
    
    std::cout << "Constructor test passed: x=" << std::any_cast<int>(x) << ", y=" << std::any_cast<int>(y) << std::endl;
}

int main() {
    try {
        test_simple_class();
        test_class_with_constructor();
        std::cout << "All class tests passed!" << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "Test failed with error: " << e.what() << std::endl;
        return 1;
    }
    return 0;
}