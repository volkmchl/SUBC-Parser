#include <iostream>
#include <fstream>
#include <string>
#include <string.h>
#include <iomanip>

#include "Parser.h"
#include "Tokenizer.h"
#include "Node.h"

int main(int argc, char* argv[]){
    
    //get file path
    std::string filePath;
    
    //if -ast flag included, file is 3rd argument
    if(argc > 2){
        filePath = std::string(argv[2]);
    }
    else{
        filePath = std::string(argv[1]);
    }
    
    //create tokenizer on that file
    Tokenizer* tokenizer = new Tokenizer(filePath);
    
    //create parser on that tokenizer
    Parser* parser = new Parser(tokenizer);
    
    if(argc > 2 && strcmp(argv[1], "-ast") == 0){
        parser->outputAST();
    }
    
    return 0;
}

/*
 while(tokenizer->hasNext()){
 
 std::cout << std::left << std::setw(12) << std::setfill(' ') << nextToken.token;
 if(nextToken.type == chr){
 std::cout << std::left << "\t" << getTokenType(nextToken.type) << " *************************************************************************** " << std::endl;
 }
 else{
 std::cout << std::left << "\t" << getTokenType(nextToken.type) << std::endl;
 }
 
 nextToken = tokenizer->next();
 }
 */
