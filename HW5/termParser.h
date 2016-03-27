#include "main.h"

#ifndef TERM_PARSER
#define TERM_PARSER

bool isFun(char ch);
bool isVar(char ch);



struct Node {
    string type; 
    vector < Node * > ch;
    Node (string type, vector < Node * > ch);
    //void printTerm();
    bool checkFun();
    //bool checkFun();
    bool checkVar();
};

ostream & operator <<(ostream & os, Node * v);

bool checkEqual(Node * v, Node * u);


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

    Node * parseEquation();
    Node * parseTerm();
    Node * parseFun();
    Node * parseVar();

};



#endif


