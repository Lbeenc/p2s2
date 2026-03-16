#ifndef SCANNER_H
#define SCANNER_H

#include <istream>
#include "token.h"

void initScanner(std::istream& input);
Token scanner();

#endif
