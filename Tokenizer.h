#ifndef TOKENIZER_H
#define TOKENIZER_H

#include <fstream>

enum TokenType{identifier, str, chr, integer, oper, keyword, delimiter};

std::string getTokenType(enum TokenType type);

struct Token{
    TokenType type;
    std::string token;
};


class Tokenizer{
    
public:
    Tokenizer(std::string filePath);
    Token next();
    bool hasNext();
    
    
private:
    std::ifstream fp;
    
};

#endif
