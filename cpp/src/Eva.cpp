#include "Eva.h"
#include "Parser.h"
#include <stdexcept>
#include <iostream>
#include <vector>
#include <fstream>

Eva::Eva() : Eva(createGlobalEnvironment()) {
}

Eva::Eva(std::shared_ptr<Environment> global) : global_(global) {
}

std::shared_ptr<Environment> Eva::createGlobalEnvironment() {
    auto global = Environment::create();
    
    // Constants
    global->define("null", nullptr);
    global->define("true", true);
    global->define("false", false);
    global->define("VERSION", std::string("1.0"));
    
    // Arithmetic operators
    global->define("+", NativeFunction([](const std::vector<Value>& args) -> Value {
        if (args.size() != 2) throw std::runtime_error("+ expects 2 arguments");
        auto a = std::any_cast<int>(args[0]);
        auto b = std::any_cast<int>(args[1]);
        return a + b;
    }));
    
    global->define("-", NativeFunction([](const std::vector<Value>& args) -> Value {
        if (args.size() == 1) {
            auto a = std::any_cast<int>(args[0]);
            return -a;
        } else if (args.size() == 2) {
            auto a = std::any_cast<int>(args[0]);
            auto b = std::any_cast<int>(args[1]);
            return a - b;
        }
        throw std::runtime_error("- expects 1 or 2 arguments");
    }));
    
    global->define("*", NativeFunction([](const std::vector<Value>& args) -> Value {
        if (args.size() != 2) throw std::runtime_error("* expects 2 arguments");
        auto a = std::any_cast<int>(args[0]);
        auto b = std::any_cast<int>(args[1]);
        return a * b;
    }));
    
    global->define("/", NativeFunction([](const std::vector<Value>& args) -> Value {
        if (args.size() != 2) throw std::runtime_error("/ expects 2 arguments");
        auto a = std::any_cast<int>(args[0]);
        auto b = std::any_cast<int>(args[1]);
        return a / b;
    }));
    
    global->define("%", NativeFunction([](const std::vector<Value>& args) -> Value {
        if (args.size() != 2) throw std::runtime_error("% expects 2 arguments");
        auto a = std::any_cast<int>(args[0]);
        auto b = std::any_cast<int>(args[1]);
        return a % b;
    }));
    
    // Comparison operators
    global->define("<", NativeFunction([](const std::vector<Value>& args) -> Value {
        if (args.size() != 2) throw std::runtime_error("< expects 2 arguments");
        auto a = std::any_cast<int>(args[0]);
        auto b = std::any_cast<int>(args[1]);
        return a < b;
    }));
    
    global->define("<=", NativeFunction([](const std::vector<Value>& args) -> Value {
        if (args.size() != 2) throw std::runtime_error("<= expects 2 arguments");
        auto a = std::any_cast<int>(args[0]);
        auto b = std::any_cast<int>(args[1]);
        return a <= b;
    }));
    
    global->define(">", NativeFunction([](const std::vector<Value>& args) -> Value {
        if (args.size() != 2) throw std::runtime_error("> expects 2 arguments");
        auto a = std::any_cast<int>(args[0]);
        auto b = std::any_cast<int>(args[1]);
        return a > b;
    }));
    
    global->define(">=", NativeFunction([](const std::vector<Value>& args) -> Value {
        if (args.size() != 2) throw std::runtime_error(">= expects 2 arguments");
        auto a = std::any_cast<int>(args[0]);
        auto b = std::any_cast<int>(args[1]);
        return a >= b;
    }));
    
    global->define("==", NativeFunction([](const std::vector<Value>& args) -> Value {
        if (args.size() != 2) throw std::runtime_error("== expects 2 arguments");
        // Simple equality check for now
        try {
            auto a = std::any_cast<int>(args[0]);
            auto b = std::any_cast<int>(args[1]);
            return a == b;
        } catch (...) {
            // If not numbers, try strings
            try {
                auto a = std::any_cast<std::string>(args[0]);
                auto b = std::any_cast<std::string>(args[1]);
                return a == b;
            } catch (...) {
                return false;
            }
        }
    }));
    
    // Console output
    global->define("print", NativeFunction([](const std::vector<Value>& args) -> Value {
        for (size_t i = 0; i < args.size(); i++) {
            if (i > 0) std::cout << " ";
            try {
                auto str = std::any_cast<std::string>(args[i]);
                std::cout << str;
            } catch (...) {
                try {
                    auto num = std::any_cast<int>(args[i]);
                    std::cout << num;
                } catch (...) {
                    try {
                        auto b = std::any_cast<bool>(args[i]);
                        std::cout << (b ? "true" : "false");
                    } catch (...) {
                        std::cout << "[object]";
                    }
                }
            }
        }
        std::cout << std::endl;
        return nullptr;
    }));
    
    return global;
}

Eva::Value Eva::evalGlobal(const std::string& code) {
    auto exp = EvaParser::parse(code);
    return evalGlobal(exp);
}

Eva::Value Eva::evalGlobal(ASTNodePtr exp) {
    return eval(exp, global_);
}

Eva::Value Eva::eval(ASTNodePtr exp, std::shared_ptr<Environment> env) {
    if (env == nullptr) {
        env = global_;
    }
    
    // Numbers
    if (auto num_node = std::dynamic_pointer_cast<NumberNode>(exp)) {
        return num_node->getValue();
    }
    
    // Strings
    if (auto str_node = std::dynamic_pointer_cast<StringNode>(exp)) {
        return std::any_cast<std::string>(str_node->toValue());
    }
    
    // Symbols (variables)
    if (auto sym_node = std::dynamic_pointer_cast<SymbolNode>(exp)) {
        return env->lookup(sym_node->getName());
    }
    
    // Lists (S-expressions)
    if (auto list_node = std::dynamic_pointer_cast<ListNode>(exp)) {
        if (list_node->empty()) {
            return std::vector<Value>{}; // empty list
        }
        
        const auto& elements = list_node->getElements();
        auto first = std::dynamic_pointer_cast<SymbolNode>(elements[0]);
        
        // If the first element is a symbol, check for special forms
        if (first) {
            const std::string& tag = first->getName();
            
            // Variable declaration: (var x 10)
        if (tag == "var") {
            if (elements.size() != 3) {
                throw std::runtime_error("var expects 2 arguments");
            }
            auto name_node = std::dynamic_pointer_cast<SymbolNode>(elements[1]);
            if (!name_node) {
                throw std::runtime_error("Variable name must be a symbol");
            }
            auto value = eval(elements[2], env);
            return env->define(name_node->getName(), value);
        }
        
        // Assignment: (set x 20) or (set (prop obj key) value)
        if (tag == "set") {
            if (elements.size() != 3) {
                throw std::runtime_error("set expects 2 arguments");
            }
            
            // Check if we're setting a property: (set (prop obj key) value)
            if (auto ref_list = std::dynamic_pointer_cast<ListNode>(elements[1])) {
                if (!ref_list->empty()) {
                    auto ref_first = std::dynamic_pointer_cast<SymbolNode>(ref_list->getElements()[0]);
                    if (ref_first && ref_first->getName() == "prop") {
                        // Property assignment: (set (prop instance propName) value)
                        if (ref_list->size() != 3) {
                            throw std::runtime_error("prop in set expects 2 arguments");
                        }
                        auto instanceEnv = std::any_cast<std::shared_ptr<Environment>>(
                            eval(ref_list->getElements()[1], env));
                        auto prop_name = std::dynamic_pointer_cast<SymbolNode>(ref_list->getElements()[2]);
                        if (!prop_name) {
                            throw std::runtime_error("Property name must be a symbol");
                        }
                        auto value = eval(elements[2], env);
                        return instanceEnv->define(prop_name->getName(), value);
                    }
                }
            }
            
            // Regular variable assignment
            auto name_node = std::dynamic_pointer_cast<SymbolNode>(elements[1]);
            if (!name_node) {
                throw std::runtime_error("Variable name must be a symbol");
            }
            auto value = eval(elements[2], env);
            return env->assign(name_node->getName(), value);
        }
        
        // Block: (begin exp1 exp2 ...)
        if (tag == "begin") {
            std::vector<ASTNodePtr> block_expressions(elements.begin() + 1, elements.end());
            return evalBlock(block_expressions, env);
        }
        
        // If expression: (if condition consequent alternate)
        if (tag == "if") {
            if (elements.size() != 4) {
                throw std::runtime_error("if expects 3 arguments");
            }
            auto condition = eval(elements[1], env);
            bool cond_result = false;
            try {
                cond_result = std::any_cast<bool>(condition);
            } catch (...) {
                // Non-bool values are truthy except for explicit false
                cond_result = true;
            }
            
            if (cond_result) {
                return eval(elements[2], env);
            } else {
                return eval(elements[3], env);
            }
        }
        
        // While loop: (while condition body)
        if (tag == "while") {
            if (elements.size() != 3) {
                throw std::runtime_error("while expects 2 arguments");
            }
            Value result = nullptr;
            while (true) {
                auto condition = eval(elements[1], env);
                bool cond_result = false;
                try {
                    cond_result = std::any_cast<bool>(condition);
                } catch (...) {
                    cond_result = true;
                }
                if (!cond_result) break;
                result = eval(elements[2], env);
            }
            return result;
        }
        
        // Lambda function: (lambda (x y) (+ x y))
        if (tag == "lambda") {
            if (elements.size() != 3) {
                throw std::runtime_error("lambda expects 2 arguments");
            }
            auto params_node = std::dynamic_pointer_cast<ListNode>(elements[1]);
            if (!params_node) {
                throw std::runtime_error("Lambda parameters must be a list");
            }
            
            std::vector<std::string> params;
            for (const auto& param : params_node->getElements()) {
                auto param_sym = std::dynamic_pointer_cast<SymbolNode>(param);
                if (!param_sym) {
                    throw std::runtime_error("Lambda parameter must be a symbol");
                }
                params.push_back(param_sym->getName());
            }
            
            UserFunction func;
            func.params = params;
            func.body = elements[2];
            func.env = env; // closure
            
            return func;
        }
        
        // Function declaration: (def square (x) (* x x))
        // Syntactic sugar for: (var square (lambda (x) (* x x)))
        if (tag == "def") {
            auto varExp = Transformer::transformDefToLambda(elements);
            return eval(varExp, env);
        }
        
        // Switch expression: (switch (cond1 block1) ...)
        // Syntactic sugar for nested if-expression
        if (tag == "switch") {
            auto ifExp = Transformer::transformSwitchToIf(elements);
            return eval(ifExp, env);
        }
        
        // For loop: (for init condition modifier body)
        // Syntactic sugar for: (begin init (while condition (begin body modifier)))
        if (tag == "for") {
            auto whileExp = Transformer::transformForToWhile(elements);
            return eval(whileExp, env);
        }
        
        // Increment: (++ var)
        if (tag == "++") {
            auto setExp = Transformer::transformIncToSet(elements);
            return eval(setExp, env);
        }
        
        // Increment with value: (+= var value)
        if (tag == "+=") {
            auto setExp = Transformer::transformIncValToSet(elements);
            return eval(setExp, env);
        }
        
        // Decrement: (-- var)
        if (tag == "--") {
            auto setExp = Transformer::transformDecToSet(elements);
            return eval(setExp, env);
        }
        
        // Decrement with value: (-= var value)
        if (tag == "-=") {
            auto setExp = Transformer::transformDecValToSet(elements);
            return eval(setExp, env);
        }
        
        // Class declaration: (class Name Parent Body)
        if (tag == "class") {
            if (elements.size() != 4) {
                throw std::runtime_error("class expects 3 arguments");
            }
            auto name_node = std::dynamic_pointer_cast<SymbolNode>(elements[1]);
            if (!name_node) {
                throw std::runtime_error("Class name must be a symbol");
            }
            const std::string& name = name_node->getName();
            
            // Evaluate parent (can be null)
            std::shared_ptr<Environment> parentEnv = nullptr;
            auto parent = eval(elements[2], env);
            try {
                parentEnv = std::any_cast<std::shared_ptr<Environment>>(parent);
            } catch (...) {
                // Parent is null, use current environment as parent
                parentEnv = env;
            }
            
            // Create class environment
            auto classEnv = Environment::create({}, parentEnv);
            
            // Evaluate class body in class environment
            eval(elements[3], classEnv);
            
            // Define class in current environment
            return env->define(name, classEnv);
        }
        
        // Class instantiation: (new ClassName args...)
        if (tag == "new") {
            if (elements.size() < 2) {
                throw std::runtime_error("new expects at least 1 argument");
            }
            
            auto classEnv = std::any_cast<std::shared_ptr<Environment>>(eval(elements[1], env));
            auto instanceEnv = Environment::create({}, classEnv);
            
            // Collect constructor arguments
            std::vector<Value> args;
            args.push_back(instanceEnv); // 'this' parameter
            for (size_t i = 2; i < elements.size(); i++) {
                args.push_back(eval(elements[i], env));
            }
            
            // Call constructor if it exists
            try {
                auto constructor = classEnv->lookup("constructor");
                callUserDefinedFunction(constructor, args);
            } catch (...) {
                // No constructor is fine
            }
            
            return instanceEnv;
        }
        
        // Property access: (prop instance name)
        if (tag == "prop") {
            if (elements.size() != 3) {
                throw std::runtime_error("prop expects 2 arguments");
            }
            auto instanceEnv = std::any_cast<std::shared_ptr<Environment>>(eval(elements[1], env));
            auto name_node = std::dynamic_pointer_cast<SymbolNode>(elements[2]);
            if (!name_node) {
                throw std::runtime_error("Property name must be a symbol");
            }
            return instanceEnv->lookup(name_node->getName());
        }
        
        // Super expression: (super ClassName)
        if (tag == "super") {
            if (elements.size() != 2) {
                throw std::runtime_error("super expects 1 argument");
            }
            auto classEnv = std::any_cast<std::shared_ptr<Environment>>(eval(elements[1], env));
            return classEnv->getParent();
        }
        
        // Module declaration: (module name body)
        if (tag == "module") {
            if (elements.size() != 3) {
                throw std::runtime_error("module expects 2 arguments");
            }
            auto name_node = std::dynamic_pointer_cast<SymbolNode>(elements[1]);
            if (!name_node) {
                throw std::runtime_error("Module name must be a symbol");
            }
            const std::string& name = name_node->getName();
            
            // Create module environment
            auto moduleEnv = Environment::create({}, env);
            
            // Evaluate module body in module environment
            eval(elements[2], moduleEnv);
            
            // Define module in current environment
            return env->define(name, moduleEnv);
        }
        
        // Module import: (import name)
        if (tag == "import") {
            if (elements.size() != 2) {
                throw std::runtime_error("import expects 1 argument");
            }
            auto name_node = std::dynamic_pointer_cast<SymbolNode>(elements[1]);
            if (!name_node) {
                throw std::runtime_error("Module name must be a symbol");
            }
            const std::string& name = name_node->getName();
            
            // Read module file
            std::string filename = "modules/" + name + ".eva";
            std::ifstream file(filename);
            if (!file.is_open()) {
                throw std::runtime_error("Cannot open module file: " + filename);
            }
            
            std::string moduleSrc((std::istreambuf_iterator<char>(file)),
                                 std::istreambuf_iterator<char>());
            file.close();
            
            // Parse and evaluate as module
            auto body = EvaParser::parse("(begin " + moduleSrc + ")");
            std::vector<ASTNodePtr> moduleElements = {
                makeSymbol("module"),
                makeSymbol(name),
                body
            };
            auto moduleExp = makeList(moduleElements);
            
            return eval(moduleExp, global_);
        }
        
        } // End of special forms (if first element is a symbol)
        
        // Function calls: (+ 1 2), (print "hello"), ((prop obj func) args...)
        auto fn_value = eval(elements[0], env);
        std::vector<Value> args;
        for (size_t i = 1; i < elements.size(); i++) {
            args.push_back(eval(elements[i], env));
        }
        
        // Native function call
        try {
            auto native_fn = std::any_cast<NativeFunction>(fn_value);
            return native_fn(args);
        } catch (...) {
            // User-defined function call
            return callUserDefinedFunction(fn_value, args);
        }
    }
    
    throw std::runtime_error("Unimplemented expression type");
}

Eva::Value Eva::evalBlock(const std::vector<ASTNodePtr>& expressions, std::shared_ptr<Environment> env) {
    Value result = nullptr;
    for (const auto& exp : expressions) {
        result = eval(exp, env);
    }
    return result;
}

Eva::Value Eva::callUserDefinedFunction(const Value& fn, const std::vector<Value>& args) {
    try {
        auto user_fn = std::any_cast<UserFunction>(fn);
        
        if (args.size() != user_fn.params.size()) {
            throw std::runtime_error("Function called with wrong number of arguments");
        }
        
        Environment::Record activation_record;
        for (size_t i = 0; i < user_fn.params.size(); i++) {
            activation_record[user_fn.params[i]] = args[i];
        }
        
        auto activation_env = Environment::create(activation_record, user_fn.env);
        return eval(user_fn.body, activation_env);
    } catch (const std::bad_any_cast&) {
        throw std::runtime_error("Not a function");
    }
}

// Helper functions for creating AST nodes (used in module import)
ASTNodePtr Eva::makeSymbol(const std::string& name) {
    return std::make_shared<SymbolNode>(name);
}

ASTNodePtr Eva::makeList(const std::vector<ASTNodePtr>& elements) {
    return std::make_shared<ListNode>(elements);
}