#include <iostream>
#include <cassert>
#include <any>
#include "../include/Eva.h"

void test_simple_class() {
    Eva eva;
    
    // Test simple class with constructor and method
    eva.evalGlobal("(class Point null (begin (def constructor (this x y) (begin (set (prop this x) x) (set (prop this y) y))) (def calc (this) (+ (prop this x) (prop this y)))))");
    
    // Create instance
    eva.evalGlobal("(var p (new Point 10 20))");
    
    // Test property access
    auto x = eva.evalGlobal("(prop p x)");
    assert(std::any_cast<int>(x) == 10);
    
    auto y = eva.evalGlobal("(prop p y)");
    assert(std::any_cast<int>(y) == 20);
    
    // Test method call
    auto result = eva.evalGlobal("((prop p calc) p)");
    assert(std::any_cast<int>(result) == 30);
    
    std::cout << "Simple class tests passed!" << std::endl;
}

void test_class_inheritance() {
    Eva eva;
    
    // First define the base class
    eva.evalGlobal("(class Point null (begin (def constructor (this x y) (begin (set (prop this x) x) (set (prop this y) y))) (def calc (this) (+ (prop this x) (prop this y)))))");
    
    // Define derived class
    eva.evalGlobal("(class Point3D Point (begin (def constructor (this x y z) (begin ((prop (super Point3D) constructor) this x y) (set (prop this z) z))) (def calc (this) (+ ((prop (super Point3D) calc) this) (prop this z)))))");
    
    // Create instance
    eva.evalGlobal("(var p (new Point3D 10 20 30))");
    
    // Test method call with inheritance
    auto result = eva.evalGlobal("((prop p calc) p)");
    assert(std::any_cast<int>(result) == 60);
    
    std::cout << "Class inheritance tests passed!" << std::endl;
}

int main() {
    test_simple_class();
    test_class_inheritance();
    
    std::cout << "All class tests passed!" << std::endl;
    return 0;
}