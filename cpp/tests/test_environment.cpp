#include <iostream>
#include <cassert>
#include "../include/Environment.h"

void test_environment() {
    // Test creating environment
    auto env = Environment::create();
    
    // Test define and lookup
    env->define("x", 42);
    auto value = std::any_cast<int>(env->lookup("x"));
    assert(value == 42);
    
    // Test assign
    env->assign("x", 100);
    value = std::any_cast<int>(env->lookup("x"));
    assert(value == 100);
    
    // Test parent environment
    auto child = Environment::create({}, env);
    child->define("y", 200);
    
    // Child can access parent variable
    value = std::any_cast<int>(child->lookup("x"));
    assert(value == 100);
    
    // Child has its own variable
    value = std::any_cast<int>(child->lookup("y"));
    assert(value == 200);
    
    // Test error for undefined variable
    try {
        env->lookup("undefined");
        assert(false); // Should not reach here
    } catch (const std::runtime_error&) {
        // Expected
    }
    
    std::cout << "Environment tests passed!" << std::endl;
}

int main() {
    test_environment();
    return 0;
}