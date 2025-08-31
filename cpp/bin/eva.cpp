#include <iostream>
#include <fstream>
#include <string>
#include "../include/Eva.h"

void printUsage() {
    std::cout << "Usage:" << std::endl;
    std::cout << "  eva -e \"expression\"  # Evaluate expression" << std::endl;
    std::cout << "  eva -f filename       # Evaluate file" << std::endl;
}

int main(int argc, char* argv[]) {
    if (argc < 3) {
        printUsage();
        return 1;
    }
    
    std::string mode = argv[1];
    std::string input = argv[2];
    
    Eva eva;
    
    try {
        Eva::Value result;
        if (mode == "-e") {
            // Direct expression
            result = eva.evalGlobal(input);
        } else if (mode == "-f") {
            // Eva file
            std::ifstream file(input);
            if (!file.is_open()) {
                std::cerr << "Error: Cannot open file " << input << std::endl;
                return 1;
            }
            
            std::string content((std::istreambuf_iterator<char>(file)),
                              std::istreambuf_iterator<char>());
            file.close();
            
            result = eva.evalGlobal("(begin " + content + ")");
        } else {
            std::cerr << "Error: Unknown mode " << mode << std::endl;
            printUsage();
            return 1;
        }
        
        // Print the result
        if (result.has_value()) {
            try {
                auto str = std::any_cast<std::string>(result);
                std::cout << "\"" << str << "\"" << std::endl;
            } catch (...) {
                try {
                    auto num = std::any_cast<int>(result);
                    std::cout << num << std::endl;
                } catch (...) {
                    try {
                        auto b = std::any_cast<bool>(result);
                        std::cout << (b ? "true" : "false") << std::endl;
                    } catch (...) {
                        try {
                            auto vec = std::any_cast<std::vector<Eva::Value>>(result);
                            std::cout << "(";
                            for (size_t i = 0; i < vec.size(); i++) {
                                if (i > 0) std::cout << " ";
                                try {
                                    auto str = std::any_cast<std::string>(vec[i]);
                                    std::cout << "\"" << str << "\"";
                                } catch (...) {
                                    try {
                                        auto num = std::any_cast<int>(vec[i]);
                                        std::cout << num;
                                    } catch (...) {
                                        try {
                                            auto b = std::any_cast<bool>(vec[i]);
                                            std::cout << (b ? "true" : "false");
                                        } catch (...) {
                                            std::cout << "[object]";
                                        }
                                    }
                                }
                            }
                            std::cout << ")" << std::endl;
                        } catch (...) {
                            std::cout << "[object]" << std::endl;
                        }
                    }
                }
            }
        } else {
            std::cout << "null" << std::endl;
        }
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
    
    return 0;
}