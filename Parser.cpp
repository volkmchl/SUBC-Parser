
#include "Parser.h"
#include <iostream>

Parser::Parser(Tokenizer* tokenizer){
    this->tokenizer = tokenizer;
    this->nextToken = tokenizer->next();
    Tiny();
}

void Parser::outputAST(){
    preOrder(stack.top(), 0);
}

//************************************************************************************************
//************************************************************************************************
void Parser::Tiny(){
    consume("program");
    
    Name();
    
    consume(":");
    
    Consts();
    Types();
    Dclns();
    SubProgs();
    Body();
    Name();
    
    consume(".");
    
    buildTree("program",7);
}
//************************************************************************************************
//************************************************************************************************
void Parser::Consts(){
    if(nextToken.token == "const"){
        consume("const");
        int n = 1;
        Const();
        while (nextToken.token == ",") {
            consume(",");
            n++;
            Const();
        }
        
        consume(";");
        buildTree("consts", n);
        
    }
    else{
        buildTree("consts", 0);
    }
}
//************************************************************************************************
//************************************************************************************************
void Parser::Const(){
    Name();
    consume("=");
    ConstValue();
    
    buildTree("const", 2);
}
//************************************************************************************************
//************************************************************************************************
void Parser::ConstValue(){
    if(nextToken.type == chr || nextToken.type == integer){
        consume(nextToken.type);
    }
    else if(nextToken.type == identifier){
        Name();
    }
    else{
        std::cout << "ERROR in CONSTVALUE()" << std::endl;
        std::cout << "TOKEN IN CONSTVALUE() " << nextToken.token << " of TYPE " << getTokenType(nextToken.type) << std::endl;
        exit(1);
    }
}
//************************************************************************************************
//************************************************************************************************
void Parser::Types(){
    if(nextToken.token == "type"){
        consume("type");
        
        int n = 0;
        while(nextToken.type == identifier){
            Type();
            consume(";");
            n++;
        }
        buildTree("types", n);
    }
    
    else{
        buildTree("types", 0);
    }
}
//************************************************************************************************
//************************************************************************************************
void Parser::Type(){
    Name();
    consume("=");
    LitList();
    
    buildTree("type", 2);
}
//************************************************************************************************
//************************************************************************************************
void Parser::LitList(){
    consume("(");
    int n = 1;
    Name();
    
    while(nextToken.token == ","){
        consume(",");
        Name();
        n++;
    }
    consume(")");
    buildTree("lit", n);
}
//************************************************************************************************
//************************************************************************************************
void Parser::SubProgs(){
    int n = 0;
    while(nextToken.token == "function"){
        Fcn();
        n++;
    }
    buildTree("subprogs", n);
}
//************************************************************************************************
//************************************************************************************************
void Parser::Fcn(){
    consume("function");
    Name();
    consume("(");
    Params();
    consume(")");
    consume(":");
    Name();
    consume(";");
    Consts();
    Types();
    Dclns();
    Body();
    Name();
    consume(";");
    
    buildTree("fcn", 8);
}
//************************************************************************************************
//************************************************************************************************
void Parser::Params(){
    
    int n = 1;
    Dcln();
    while(nextToken.token == ";"){
        consume(";");
        Dcln();
        n++;
    }
    buildTree("params", n);
}
//************************************************************************************************
//************************************************************************************************
void Parser::Dclns(){
    
    if(nextToken.token == "var"){
        consume("var");
        
        int n = 1;
        Dcln();
        consume(";");
        
        while(nextToken.type == identifier){
            Dcln();
            consume(";");
            n++;
        }
        
        buildTree("dclns", n);
        
    }
    else{
        buildTree("dclns", 0);
    }
}
//************************************************************************************************
//************************************************************************************************
void Parser::Dcln(){
    
    int n = 1;
    
    Name();
    while(nextToken.token == ","){
        consume(",");
        Name();
        n++;
    }
    
    consume(":");
    Name();
    buildTree("var", n+1);
}
//************************************************************************************************
//************************************************************************************************
void Parser::Body(){
    
    consume("begin");
    
    int n = 1;
    Statement();
    while(nextToken.token == ";"){
        consume(";");
        Statement();
        n++;
    }
    consume("end");
    buildTree("block", n);
}
//************************************************************************************************
//************************************************************************************************
void Parser::Statement(){
    
    if(nextToken.type == identifier){
        Assignment();
    }
    
    else if(nextToken.token == "output"){
        consume("output");
        consume("(");
        
        int n = 1;
        OutExp();
        while(nextToken.token == ","){
            consume(",");
            OutExp();
            n++;
        }
        
        consume(")");
        buildTree("output", n);
    }
    
    else if(nextToken.token == "if"){
        
        consume("if");
        Expression();
        consume("then");
        Statement();
        
        if(nextToken.token == "else"){
            consume("else");
            Statement();
            buildTree("if", 3);
        }
        
        else{
            buildTree("if", 2);
        }
        
    }
    
    else if(nextToken.token == "while"){
        consume("while");
        Expression();
        consume("do");
        Statement();
        
        buildTree("while", 2);
    }
    
    else if(nextToken.token == "repeat"){
        consume("repeat");
        
        int n = 1;
        Statement();
        while(nextToken.token == ";"){
            consume(";");
            Statement();
            n++;
        }
        
        consume("until");
        Expression();
        
        buildTree("repeat", n+1);
    }
    
    else if(nextToken.token == "for"){
        consume("for");
        consume("(");
        ForStat();
        consume(";");
        ForExp();
        consume(";");
        ForStat();
        consume(")");
        Statement();
        
        buildTree("for", 4);
    }
    
    else if(nextToken.token == "loop"){
        consume("loop");
        
        int n = 1;
        Statement();
        while(nextToken.token == ";"){
            consume(";");
            Statement();
            n++;
        }
        
        consume("pool");
        buildTree("loop", n);
        
    }
    
    else if(nextToken.token == "case"){
        int n = 1;
        consume("case");
        Expression();
        consume("of");
        n += Caseclauses();
        n += OtherwiseClause();
        consume("end");
        
        buildTree("case", n);
    }
    
    else if(nextToken.token == "read"){
        
        consume("read");
        consume("(");
        
        int n = 1;
        Name();
        while(nextToken.token == ","){
            consume(",");
            Name();
            n++;
        }
        
        consume(")");
        buildTree("read", n);
        
    }
    
    else if(nextToken.token == "exit"){
        consume("exit");
        buildTree("exit", 0);
    }
    
    else if(nextToken.token == "return"){
        consume("return");
        Expression();
        
        buildTree("return", 1);
    }
    
    else if(nextToken.token == "begin"){
        Body();
    }
    
    else{
        buildTree("<null>", 0);
    }
}
//************************************************************************************************
//************************************************************************************************
void Parser::OutExp(){
    
    if(nextToken.type == str){
        StringNode();
        buildTree("string", 1);
    }
    else{
        Expression();
        buildTree("integer", 1);
    }
}
//************************************************************************************************
//************************************************************************************************
void Parser::StringNode(){
    consume(str);
}
//************************************************************************************************
//************************************************************************************************
int Parser::Caseclauses(){
    
    int n = 1;
    Caseclause();
    consume(";");
    
    while(nextToken.type == identifier || nextToken.type == chr || nextToken.type == integer){
        Caseclause();
        n++;
        consume(";");
    }
    return n;
}
//************************************************************************************************
//************************************************************************************************
void Parser::Caseclause(){
    
    int n = 1;
    
    CaseExpression();
    while(nextToken.token == ","){
        consume(",");
        CaseExpression();
        n++;
    }
    
    consume(":");
    Statement();
    
    buildTree("case_clause", n+1);
}
//************************************************************************************************
//************************************************************************************************
void Parser::CaseExpression(){
    
    ConstValue();
    
    if(nextToken.token == ".."){
        consume("..");
        ConstValue();
        buildTree("..", 2);
    }
}
//************************************************************************************************
//************************************************************************************************
int Parser::OtherwiseClause(){
    
    if(nextToken.token == "otherwise"){
        consume("otherwise");
        Statement();
        buildTree("otherwise", 1);
        return 1;
    }
    return 0;
    
}
//************************************************************************************************
//************************************************************************************************
void Parser::Assignment(){
    
    Name();
    
    if(nextToken.token == ":="){
        consume(":=");
        Expression();
        buildTree("assign", 2);
    }
    else if(nextToken.token == ":=:"){
        consume(":=:");
        Name();
        buildTree("swap", 2);
    }
}
//************************************************************************************************
//************************************************************************************************
void Parser::ForStat(){
    
    if(nextToken.type == identifier){
        Assignment();
    }
    else{
        buildTree("<null>", 0);
    }
}
//************************************************************************************************
//************************************************************************************************
void Parser::ForExp(){
    if(nextToken.token == "-" || nextToken.token == "+" || nextToken.token == "not" || nextToken.token == "eof" || nextToken.token == "succ" || nextToken.token == "pred" || nextToken.token == "chr" || nextToken.token == "ord" || nextToken.token == "(" || nextToken.type == identifier || nextToken.type == integer || nextToken.type == chr){
        Expression();
    }
    else{
        buildTree("true", 0);
    }
}
//************************************************************************************************
//************************************************************************************************
void Parser::Expression(){
    
    Term();
    
    if(nextToken.token == "<="){
        consume("<=");
        Term();
        buildTree("<=", 2);
    }
    else if(nextToken.token == "<"){
        consume("<");
        Term();
        buildTree("<", 2);
    }
    else if(nextToken.token == ">="){
        consume(">=");
        Term();
        buildTree(">=", 2);
    }
    else if(nextToken.token == ">"){
        consume(">");
        Term();
        buildTree(">", 2);
    }
    else if(nextToken.token == "="){
        consume("=");
        Term();
        buildTree("=", 2);
    }
    else if(nextToken.token == "<>"){
        consume("<>");
        Term();
        buildTree("<>", 2);
    }
}
//************************************************************************************************
//************************************************************************************************
void Parser::Term(){
    
    Factor();
    while(nextToken.token == "+" || nextToken.token == "-" || nextToken.token == "or"){
        if(nextToken.token == "+"){
            consume("+");
            Factor();
            buildTree("+", 2);
        }
        
        else if (nextToken.token == "-"){
            consume("-");
            Factor();
            buildTree("-", 2);
        }
        
        else if(nextToken.token == "or"){
            consume("or");
            Factor();
            buildTree("or", 2);
        }
    }
}
//************************************************************************************************
//************************************************************************************************
void Parser::Factor(){
    
    Primary();
    while(nextToken.token == "*" || nextToken.token == "/" || nextToken.token == "and" || nextToken.token == "mod"){
        if(nextToken.token == "*"){
            consume("*");
            Primary();
            buildTree("*", 2);
        }
        else if(nextToken.token == "/"){
            consume("/");
            Primary();
            buildTree("/", 2);
        }
        else if(nextToken.token == "and"){
            consume("and");
            Primary();
            buildTree("and", 2);
        }
        else if(nextToken.token == "mod"){
            consume("mod");
            Primary();
            buildTree("mod", 2);
        }
    }
}
//************************************************************************************************
//************************************************************************************************
void Parser::Primary(){
    
    if(nextToken.type == chr){
        consume(nextToken.type);
    }
    else if(nextToken.token == "-"){
        consume("-");
        Primary();
        buildTree("-", 1);
    }
    else if(nextToken.token == "+"){
        consume("+");
        Primary();
    }
    else if(nextToken.token == "not"){
        consume("not");
        Primary();
        buildTree("not", 1);
    }
    else if(nextToken.token == "eof"){
        consume("eof");
        buildTree("eof", 0);
    }
    else if(nextToken.token == "("){
        consume("(");
        Expression();
        consume(")");
    }
    else if(nextToken.token == "succ"){
        consume("succ");
        consume("(");
        Expression();
        consume(")");
        buildTree("succ", 1);
    }
    else if(nextToken.token == "pred"){
        consume("pred");
        consume("(");
        Expression();
        consume(")");
        buildTree("pred", 1);
    }
    else if(nextToken.token == "chr"){
        consume("chr");
        consume("(");
        Expression();
        consume(")");
        buildTree("chr", 1);
    }
    else if(nextToken.token == "ord"){
        consume("ord");
        consume("(");
        Expression();
        consume(")");
        buildTree("ord", 1);
    }
    else if(nextToken.type == identifier){
        int n = 1;
        Name();
        if(nextToken.token == "("){
            consume("(");
            n++;
            Expression();
            while(nextToken.token == ","){
                consume(",");
                Expression();
                n++;
            }
            consume(")");
            buildTree("call", n);
        }
    }
    else if(nextToken.type == integer){
        consume(nextToken.type);
    }
    else{
        //std::cout << "ERROR in Primary()" << std::endl;
        //std::cout << "TOKEN IN Primary() " << nextToken.token << " of TYPE " << getTokenType(nextToken.type) << std::endl;
        exit(1);
    }
}
//************************************************************************************************
//************************************************************************************************
void Parser::Name(){
    consume(identifier);
}


void Parser::consume(enum TokenType type){
    
    if(nextToken.type != type){
        std::cout << "ERROR: expected TOKEN type identifier but next TOKEN was type " << getTokenType(nextToken.type) << std::endl;
        exit(1);
    }
    
    else if(type == integer){
        buildTree(nextToken.token, 0);
        buildTree("<integer>", 1);
    }
    
    else if(type == chr){
        buildTree("'" + nextToken.token + "'", 0);
        buildTree("<char>", 1);
    }
    
    else if(type == identifier) {
        buildTree(nextToken.token, 0);
        buildTree("<identifier>", 1);
    }
    
    else if(type == str){
        buildTree(nextToken.token, 0);
        buildTree("<string>", 1);
    }
    
    //update nextToken
    if(tokenizer->hasNext()){
        nextToken = tokenizer->next();
        //std::cout << "NEXT TOKEN: " << nextToken.token << " of type " << getTokenType(nextToken.type) <<std::endl;
    }
}



void Parser::consume(std::string token){
    
    //if token not as expected, then error
    if(!(nextToken.token == token)){
        std::cout << "ERROR: expected TOKEN <" << token << ">, but next TOKEN was " << nextToken.token << " of type " <<  getTokenType(nextToken.type) <<std::endl;
        exit(1);
    }
    
    //update nextToken
    if(tokenizer->hasNext()){
        nextToken = tokenizer->next();
    }
}

void Parser::buildTree(std::string node, int cnt){
    
    Node* root = new Node(node, cnt);
    
    Node* temp = NULL;
    for(int i = 0; i < cnt; i++){
        Node* c = this->stack.top();
        this->stack.pop();
        c->sibling = temp;
        temp = c;
    }
    
    root->child = temp;
    root->sibling = NULL;
    this->stack.push(root);
    
}

void Parser::preOrder(Node* root, int dots){
    if(root == NULL) return;
    
    for(int i = 0; i < dots; i++){
        std::cout << ". ";
    }
    
    std::cout << root->token << "(" << root->numChildren << ")" << std::endl;
    preOrder(root->child, dots+1);
    preOrder(root->sibling, dots);
}
