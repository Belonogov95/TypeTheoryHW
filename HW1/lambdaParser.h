#include "main.h"

#ifndef LABMDA_PARSER
#define LABMDA_PARSER
   



struct LexicalAnalyzer {
    string s;
    string token;
    int cur;
    LexicalAnalyzer(string s);
    void next();
    string curToken();
};

struct Node {
    string type;
    vector < Node * > ch;
    Node (string type);
    Node (string type, Node * ch1);
    Node (string type, Node * ch1, Node * ch2);
    Node (string type, Node * ch1, Node * ch2, Node * ch3);
};

struct LambdaParser {
    LexicalAnalyzer lex;
    LambdaParser(string s);
    Node * parseExp();
    Node * parseApply();
    Node * parseAtom();
    Node * parseVar();
};


#endif

