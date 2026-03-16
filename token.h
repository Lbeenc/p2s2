#ifndef TOKEN_H
#define TOKEN_H

#include <string>

enum TokenID {
    t1_tk,
    t2_tk,
    t3_tk,
    EOFtk,
    ERRORtk
};

struct Token {
    TokenID id;
    std::string instance;
    int line;
};

static const std::string tokenNames[] = {
    "t1 token",
    "t2 token",
    "t3 token",
    "EOFTk",
    "ERRORtk"
};

#endif
