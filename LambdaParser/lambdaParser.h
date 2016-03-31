#include "main.h"

#ifndef LABMDA_PARSER
#define LABMDA_PARSER
//#define LABMDA_PARSER
struct Node;
   
typedef shared_ptr < Node > SNode;

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
    long long len;
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

extern int cntCon1;
extern int cntCon2;
extern int cntDest;
extern int cntSub;

struct Node {
    string type;
    const shared_ptr < Node > l, r;
    int h;
    Hash hash;
    set < string > freeCh;
    Node (string type);
    Node (string type, shared_ptr < Node > l, shared_ptr < Node > r);
    ~Node();

private:
    void updateHash();
public:
    ull getHash();
    int getLen();
    shared_ptr < Node > getL();
    shared_ptr < Node > getR();
    bool isVar();
    bool isAbstr();
    bool isApply();
};


struct LambdaParser {
    LexicalAnalyzer lex;
    LambdaParser(string s);
    shared_ptr < Node > parseExp();
    shared_ptr < Node > parseApply();
    shared_ptr < Node > parseAtom();
    shared_ptr < Node > parseVar();
    shared_ptr < Node > parseCondition();
};
// tools


struct FreeVarGenerator {
    set < int > q;
    int cur;

    FreeVarGenerator();

    void add(shared_ptr < Node > v);
    void add(string s);
    string code(int x);
    int decode(string s);
    string next();
    
};

set < string > genFV(shared_ptr < Node > v);

//shared_ptr < Node > createCopy(shared_ptr < Node > v);

bool checkFV(shared_ptr < Node > v, string var);

shared_ptr < Node > makeSubst(shared_ptr < Node > v, string name, shared_ptr < Node > u, int & cnt, FreeVarGenerator & gen);

string genAns(shared_ptr < Node > v);

shared_ptr < Node > parse(string s);

#endif

