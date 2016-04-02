#include "../LambdaParser/main.h"

#ifndef TERM_PARSER
#define TERM_PARSER

bool isFun(char ch);
bool isVar(char ch);



struct TNode {
    string type; 
    const vector < shared_ptr < TNode > > ch;
    TNode (string type);
    TNode (string type, vector < shared_ptr < TNode > > ch);
    bool checkFun();
    bool checkVar();
};

ostream & operator <<(ostream & os, shared_ptr < TNode > v);

bool checkEqual(shared_ptr < TNode > v, shared_ptr < TNode > u);


struct LexicalAnalyzerT {
    string s;
    string token;
    int cur;

    LexicalAnalyzerT(string s);
    void next();
    string curToken();
};


struct TermParser {
    LexicalAnalyzerT lex;
    
    TermParser(string s);

    shared_ptr < TNode > parseEquation();
    shared_ptr < TNode > parseTerm();
    shared_ptr < TNode > parseFun();
    shared_ptr < TNode > parseVar();

};



#endif


