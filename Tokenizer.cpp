#include <iostream>
#include <unordered_set>
#include "Tokenizer.h"

//store keywords for classification of tokens
std::unordered_set<std::string> keywords   ({
                                           "program","var","const","type","function","return","begin","end","output","if","then","else","while","do",
                                           "case","of","otherwise","repeat","for","until","loop","pool","exit","read","succ","pred","chr","ord","eof"
                                           });

//store operators for classification of tokens
std::unordered_set<std::string> operators  ({
                                           ":=:", ":=", "..", "<=", "<>", "<", ">=", ">", "=", "mod","and","or","not",":",";",".",",","+","-","*","/"
                                           });

//store delimiters for classification of tokens
std::unordered_set<std::string> delimiters ({
                                           "{","}","(",")"
                                           });

//from enum TokenType to its string representation
std::string getTokenType(enum TokenType type){
    switch (type)
    {
        case identifier:    return "identifier";
        case str:           return "string";
        case chr:           return "char";
        case integer:       return "integer";
        case oper:          return "operator";
        case keyword:       return "keyword";
        case delimiter:     return "delimiter";
    }
}

//setTokenType prototype
void setTokenType(struct Token& token);

//Tokenizer constructor
Tokenizer::Tokenizer(std::string filePath){
    fp.open(filePath);
}

//check for more tokens
bool Tokenizer::hasNext(){
	if(!fp.eof()) return true;
    return false;
}

//implementation of next()
Token Tokenizer::next(){
    
    struct Token nextToken;
    std::string token;
    
    char c;
    while(fp.get(c)){
        
		//consume multiple line comment
        if(c == '{'){
            while(fp.get(c) && c != '}') continue;
        }
        
		//consume single line comment
        else if(c == '#'){
            while(fp.get(c) && c != '\n') continue;
        }
        
		//if ran into newline and had something in token return that token
        else if(c =='\n'){
            
            if(token.length() > 0){
                nextToken.token = token;
                setTokenType(nextToken);
                
                return nextToken;
            }
            
        }
        
		//consume newline, spaces, tabs
        else if(c == ' ' || c == '\t' || c == '\n'){
            
			//consume tabs, newlines, spaces
            while(fp.get(c) && (c == ' ' || c == '\n' || c == '\t')) continue;
            
			//consumed something that was not newline, tab or space, so back up once
            fp.unget();
            
			//after having consumed whitespace,newline, and tabs, check if there was something in token
            if(token.length() > 0 && (token != " " || token != "\n")){
                
                nextToken.token = token;
                setTokenType(nextToken);
                
                return nextToken;
            }
            
        }
        
		//add alphanumeric characters to token
        else if(isalnum(c)){
            token += c;
        }
        
		//ran into symbol ex: ;,:,(,) and there was something in token
        else if(!isalnum(c) && token.length() > 0){
            
			//if it was a space or a newline, back up
            if( c != ' ' || c != '\n' ){
                fp.unget();
            }
            
            nextToken.token = token;
            setTokenType(nextToken);
            return nextToken;
        }
        
		//ran into symbol and token was empty
        else if(!isalnum(c)){
            
            token += c;
            //handle :=,:, :=:
            if(c == ':'){
                fp.get(c);
                
                if(c == '='){
                    token += c;
                    
                    fp.get(c);
                    
                    if(c == ':'){
                        token += c;
                        
                        nextToken.token = token;
                        nextToken.type = oper;
                        
                        return nextToken;
                    }
                    
                    fp.unget();
                    nextToken.token = token;
                    nextToken.type = oper;
                    
                    return nextToken;
                    
                }
                
                fp.unget();
                nextToken.token = token;
                nextToken.type = oper;
                
                return nextToken;
            }
            
			//handle . and ..
            if(c == '.'){
                fp.get(c);
                if(c == '.'){
                    token += '.';
                    
                    nextToken.token = token;
                    nextToken.type = oper;
                    return nextToken;
                    
                }
                
                fp.unget();
                
                nextToken.token = token;
                nextToken.type = oper;
                return nextToken;
                
                
            }
            
			//handle <, <=, <>
            if(c == '<'){
                fp.get(c);
                if(c == '='){
                    token += '=';
                    nextToken.token = token;
                    nextToken.type = oper;
                    return nextToken;
                }
                else if(c == '>'){
                    token += '>';
                    nextToken.token = token;
                    nextToken.type = oper;
                    return nextToken;
                }
                
                fp.unget();
                nextToken.token = token;
                nextToken.type = oper;
                return nextToken;
                
            }
            
			//handle >,>=
            if(c == '>'){
                fp.get(c);
                
                if(c == '='){
                    token += '=';
                    nextToken.token = token;
                    nextToken.type = oper;
                    return nextToken;
                }
                
                fp.unget();
                nextToken.token = token;
                nextToken.type = oper;
                return nextToken;
            }

			//handle '
            if(c == '\''){
                
                //grab char
                fp.get(c);
                
				//only character type is enclosed in ' '
                token = c;
                nextToken.token = token;
                nextToken.type = chr;
                
                //consume end '
                fp.get(c);
                
                return nextToken;
                
            }
            
            
            nextToken.token = token;
            setTokenType(nextToken);
            return nextToken;
        }
        
        
    }
    
    nextToken.token = token;
    setTokenType(nextToken);
    
    return nextToken;
    
}

//set type of token
void setTokenType(struct Token& TOKEN){
    
	//if token is in keyword set
	if(keywords.find(TOKEN.token) != keywords.end()){
        TOKEN.type = keyword;
    }

	//if token is in operators set
    else if (operators.find(TOKEN.token) != operators.end()){
        TOKEN.type = oper;
    }

	//if token is in delimiters set
    else if(delimiters.find(TOKEN.token) != delimiters.end()){
        TOKEN.type = delimiter;
    }

	//everything else
    else{

		//one character long
        if(TOKEN.token.length() == 1){
            //digit then integer
			if(isdigit(TOKEN.token.at(0))){
                TOKEN.type = integer;
            }
			//otherwise identifier
            else{
                TOKEN.type = identifier;
            }
        }
        
		//more than a character long
		else if(TOKEN.token.length() > 1){
            
			//check if token contains letters
            bool containsLetter = false;
            for(int i = 0; i < TOKEN.token.length(); i++){
                if(isalpha(TOKEN.token.at(i))){
                    containsLetter = true;
                    break;
                }
            }
            
			//if it does it's an identifier
            if(containsLetter){
                TOKEN.type = identifier;
            }

			//otherwise it's an integer 
            else{
                TOKEN.type = integer;
            }
        }
    }
}
