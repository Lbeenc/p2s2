#include "testScanner.h"

#include <iostream>
#include "scanner.h"
#include "token.h"

void testScanner() {
    while (true) {
        Token tk = scanner();
        if (tk.id == EOFtk) {
            std::cout << tokenNames[EOFtk] << std::endl;
            break;
        }
        std::cout << tokenNames[tk.id] << " " << tk.instance << " " << tk.line << std::endl;
    }
}
