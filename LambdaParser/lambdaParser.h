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
ull binPow(ull a, int b) ;

struct Hash {
    int len;
    ull hash;
    Hash(string s) {
        len = s.size();
        hash = 0;
        for (auto ch: s)
            hash = hash * P + ch;
    }
    Hash(int len, ull hash): len(len), hash(hash) { }
    Hash():len(0), hash(0) { }
    Hash operator + (Hash other) {
        return Hash(len + other.len, hash * binPow(P, other.len) + other.hash);  
    }
};

struct Node {
    string type;
private:
    Node * l, * r;
    Hash hash;
public:
    Node (string type);
    Node (string type, Node * l, Node * r);

private:
    void updateHash();
public:
    ull getHash();
    Node * getL();
    Node * getR();
    void setL(Node * v);
    void setR(Node * v);
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

Node * createCopy(Node * v);

bool checkFV(Node * v, string var);

Node * makeSubst(Node * v, string name, Node * u, int & cnt, FreeVarGenerator & gen);

string genAns(Node * v);

Node * parse(string s);

#endif

