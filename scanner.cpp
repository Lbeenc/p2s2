#include "scanner.h"

#include <cctype>
#include <cstdlib>
#include <iostream>
#include <sstream>
#include <string>

namespace {
    std::istream* in = nullptr;
    int lineNumber = 1;
    int lookahead = ' ';

    bool isT3Char(int ch) {
        return ch >= 36 && ch <= 47;
    }

    int getColumn(int ch) {
        if (std::isalpha(static_cast<unsigned char>(ch))) return 0;
        if (std::isdigit(static_cast<unsigned char>(ch))) return 1;
        if (ch == '"' || ch == '#') return 2;
        if (isT3Char(ch)) return 3;
        if (std::isspace(static_cast<unsigned char>(ch))) return 4;
        if (ch == EOF) return 5;
        return 6;
    }

    void scannerError(const std::string& bad) {
        std::cerr << "SCANNER ERROR: " << bad << " line " << lineNumber << std::endl;
        std::exit(1);
    }

    void readNext() {
        lookahead = in->get();
    }

    void skipIgnored() {
        while (true) {
            while (lookahead != EOF && std::isspace(static_cast<unsigned char>(lookahead))) {
                if (lookahead == '\n') {
                    ++lineNumber;
                }
                readNext();
            }

            if (lookahead == '!') {
                int commentStartLine = lineNumber;
                readNext();
                while (lookahead != EOF && lookahead != '!') {
                    if (lookahead == '\n') {
                        ++lineNumber;
                    }
                    readNext();
                }
                if (lookahead == EOF) {
                    std::cerr << "SCANNER ERROR: unterminated comment line " << commentStartLine << std::endl;
                    std::exit(1);
                }
                readNext();
                continue;
            }
            break;
        }
    }
}

void initScanner(std::istream& input) {
    in = &input;
    lineNumber = 1;
    lookahead = ' ';
    readNext();
}

Token scanner() {
    if (in == nullptr) {
        std::cerr << "SCANNER ERROR: scanner not initialized line 1" << std::endl;
        std::exit(1);
    }

    skipIgnored();

    Token token;
    token.instance = "";
    token.line = lineNumber;

    if (lookahead == EOF) {
        token.id = EOFtk;
        token.instance = "EOF";
        return token;
    }

    // Deterministic FSA lookup table
    // Columns: 0=letter, 1=digit, 2='"'|#, 3=t3 char, 4=ws, 5=eof, 6=other
    // States: 0=start, 1=letters, 2=letters+digits, 3=prefix('"'|#), 4=t1 digits
    const int FSA[5][7] = {
        {1, -1, 3, -4, 0, -5, -1},
        {1,  2, -2, -2, -2, -2, -2},
        {-1, 2, -3, -3, -3, -3, -3},
        {-1, 4, -1, -1, -1, -1, -1},
        {-1, 4, -4, -4, -4, -4, -4}
    };

    int state = 0;

    while (true) {
        int col = getColumn(lookahead);
        int next = FSA[state][col];

        if (next >= 0) {
            if (state == 0 && next == 0) {
                readNext();
                continue;
            }
            token.instance += static_cast<char>(lookahead);
            state = next;
            readNext();
            continue;
        }

        // negative values encode accept/error actions
        if (state == 0 && next == -4) {
            token.instance += static_cast<char>(lookahead);
            token.id = t3_tk;
            readNext();
            return token;
        }

        if (state == 1 && next == -2) {
            scannerError(token.instance);
        }

        if (state == 2 && next == -3) {
            token.id = t2_tk;
            return token;
        }

        if (state == 4 && next == -4) {
            token.id = t1_tk;
            return token;
        }

        if (state == 0 && next == -5) {
            token.id = EOFtk;
            token.instance = "EOF";
            return token;
        }

        if (state == 0) {
            std::string bad(1, static_cast<char>(lookahead));
            scannerError(bad);
        }

        scannerError(token.instance);
    }
}
