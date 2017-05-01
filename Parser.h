#ifndef PARSER_H
#define PARSER_H

#include <stack>
#include "Tokenizer.h"
#include "Node.h"

class Parser{
    
public:
    Tokenizer* tokenizer;
    struct Token nextToken;
    std::stack<Node*> stack;
    
    void consume(std::string token);
    void consume(enum TokenType type);
    void buildTree(std::string node, int cnt);
    void preOrder(Node* root, int dots);
    void outputAST();
    
    void Tiny();
    void Name();
    void Consts();
    void Types();
    void Dclns();
    void SubProgs();
    void Body();
    void Const();
    void ConstValue();
    void Type();
    void LitList();
    void Fcn();
    void Params();
    void Dcln();
    void Statement();
    void Assignment();
    void Expression();
    void Term();
    void Factor();
    void Primary();
    void OutExp();
    void StringNode();
    void CaseExpression();
    int Caseclauses();
    void Caseclause();
    void ForStat();
    void ForExp();
    int OtherwiseClause();

    Parser(Tokenizer* tokenizer);
};

#endif
