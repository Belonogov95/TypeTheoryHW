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
    Node * l, * r;
    Node (string type);
    Node (string type, Node * l, Node * r);
};

struct LambdaParser {
    LexicalAnalyzer lex;
    LambdaParser(string s);
    Node * parseExp();
    Node * parseApply();
    Node * parseAtom();
    Node * parseVar();
    Node * parseCondition();
};
// tools


struct FreeVarGenerator {
    set < int > q;
    int cur;

    FreeVarGenerator();

    void add(Node * v);
    void add(string s);
    string code(int x);
    int decode(string s);
    string next();
    
};

//set < string > genFV(Node * v);

bool checkFV(Node * v, string var);

Node * makeSubst(Node * v, string name, Node * u, int & cnt, FreeVarGenerator & gen);

string genAns(Node * v);

Node * parse(string s);

#endif

