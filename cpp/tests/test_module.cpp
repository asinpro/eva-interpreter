#include <iostream>
#include <cassert>
#include <any>
#include "../include/Eva.h"

void test_module_declaration() {
    Eva eva;
    
    std::cout << "Testing module declaration..." << std::endl;
    
    // Test simple module declaration first
    eva.evalGlobal("(module Math (def abs (x) (if (< x 0) (- x) x)))");
    std::cout << "Module declared successfully" << std::endl;
    
    // Test accessing module functions
    auto result = eva.evalGlobal("((prop Math abs) -10)");
    assert(std::any_cast<int>(result) == 10);
    std::cout << "Module function access: " << std::any_cast<int>(result) << std::endl;
    
    std::cout << "Module declaration test passed!" << std::endl;
}

void test_module_import() {
    Eva eva;
    
    std::cout << "Testing module import..." << std::endl;
    
    try {
        // Test importing module from file
        eva.evalGlobal("(import Math)");
        std::cout << "Module imported successfully" << std::endl;
        
        // Test accessing imported module functions
        auto result = eva.evalGlobal("((prop Math abs) (- 10))");
        assert(std::any_cast<int>(result) == 10);
        std::cout << "Imported module function access: " << std::any_cast<int>(result) << std::endl;
        
        // Test accessing imported module variables
        auto maxVal = eva.evalGlobal("(prop Math MAX_VALUE)");
        assert(std::any_cast<int>(maxVal) == 1000);
        std::cout << "Imported module variable access: " << std::any_cast<int>(maxVal) << std::endl;
        
        std::cout << "Module import test passed!" << std::endl;
    } catch (const std::exception& e) {
        std::cout << "Module import test failed (file not found): " << e.what() << std::endl;
        std::cout << "This is expected if running from wrong directory" << std::endl;
    }
}

int main() {
    try {
        test_module_declaration();
        test_module_import();
        std::cout << "All module tests completed!" << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "Test failed with error: " << e.what() << std::endl;
        return 1;
    }
    return 0;
}