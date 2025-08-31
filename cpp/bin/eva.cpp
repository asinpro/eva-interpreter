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
        if (mode == "-e") {
            // Direct expression
            eva.evalGlobal(input);
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
            
            eva.evalGlobal("(begin " + content + ")");
        } else {
            std::cerr << "Error: Unknown mode " << mode << std::endl;
            printUsage();
            return 1;
        }
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
    
    return 0;
}