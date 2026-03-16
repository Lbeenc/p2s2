#include <fstream>
#include <iostream>
#include <string>

#include "scanner.h"
#include "testScanner.h"

int main(int argc, char* argv[]) {
    if (argc > 2) {
        std::cerr << "Usage: P2 [file]" << std::endl;
        return 1;
    }

    if (argc == 2) {
        std::ifstream infile(argv[1]);
        if (!infile.is_open()) {
            std::cerr << "Program error file not found" << std::endl;
            return 1;
        }
        initScanner(infile);
        testScanner();
    } else {
        initScanner(std::cin);
        testScanner();
    }

    return 0;
}
