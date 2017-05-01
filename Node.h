#ifndef NODE_H
#define NODE_H

#include "Tokenizer.h"

class Node{
public:
    std::string token;
    int numChildren;
    
    Node *root, *child, *sibling;
    Node (std::string node, int numChildren);
};

#endif
