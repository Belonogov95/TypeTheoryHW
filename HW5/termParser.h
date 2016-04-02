#include "main.h"

#ifndef TERM_PARSER
#define TERM_PARSER

bool isFun(char ch);
bool isVar(char ch);



struct Node {
    string type; 
    const vector < shared_ptr < Node > > ch;
    Node (string type, vector < shared_ptr < Node > > ch);
    bool checkFun();
    bool checkVar();
};

ostream & operator <<(ostream & os, shared_ptr < Node > v);

bool checkEqual(shared_ptr < Node > v, shared_ptr < Node > u);


struct LexicalAnalyzer {
    string s;
    string token;
    int cur;

    LexicalAnalyzer(string s);
    void next();
    string curToken();
};


struct TermParser {
    LexicalAnalyzer lex;
    
    TermParser(string s);

    shared_ptr < Node > parseEquation();
    shared_ptr < Node > parseTerm();
    shared_ptr < Node > parseFun();
    shared_ptr < Node > parseVar();

};



#endif


