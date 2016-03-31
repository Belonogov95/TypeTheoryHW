#include "main.h"
#include "lambdaParser.h"




LexicalAnalyzer::LexicalAnalyzer(string s): s(s), cur(0) { }

bool myCheck(char ch) {
    if ('a' <= ch && ch <= 'z') return 1;
    if ('0' <= ch && ch <= '9') return 1;
    if (ch == '`') return 1;
    return 0;
}

ull binPow(ull a, int b) {
    ull res = 1;
    for (; b > 0; b /= 2) {
        if (b % 2 == 1) 
            res = res * a;
        a = a * a;
    }
    return res;
}

void LexicalAnalyzer::next() {
    for (; cur < (int)s.size() && isspace(s[cur]); cur++);

    if (cur == (int)s.size()) {
        token = "";
        return;
    } 

    if (s[cur] == '\\' || s[cur] == '.' || s[cur] == '(' || s[cur] == ')' || s[cur] == '[' || s[cur] == ']') {
        token = s.substr(cur, 1); 
        cur++;
        return;
    }
    if (cur + 2 <= (int)s.size() && s.substr(cur, 2) == ":=") {
        token = s.substr(cur, 2);
        cur += 2;
        return;
    }
    assert(isalpha(s[cur]));
    int t = cur;
    for (; cur < (int)s.size() && myCheck(s[cur]); cur++);

    token = s.substr(t, cur - t);
}

int cntCon1;
int cntCon2;
int cntDest;

Node :: Node (string type): type(type), l(NULL), r(NULL), h(0), hash(type) {
    freeCh.insert(type);
    cntCon1++;
}

Node :: Node (string type, shared_ptr < Node > l, shared_ptr < Node > r): type(type), l(l), r(r) { 
    updateHash();
    if (isApply()) {
        freeCh = l->freeCh;
        freeCh.insert(r->freeCh.begin(), r->freeCh.end());
    }
    else {
        freeCh = r->freeCh;
        freeCh.erase(l->type);
    }
    assert(freeCh.size() < 20);
    cntCon2++;
}
Node :: ~Node() {
    cntDest++;
}

bool Node::isVar() {
    return islower(type[0]);         
}

bool Node::isAbstr() {
    return type == "ABSTR";
}

bool Node::isApply() {
    return type == "APPLY";
}

ull Node::getHash() {
    return hash.hash;
}

int Node::getLen() {
    return hash.len;
}

void Node::updateHash() {
    h = 0;
    if (l != NULL) h = max(h, l->h + 1);
    if (r != NULL) h = max(h, r->h + 1);
    if (type == "APPLY") {
        hash = Hash("(") + l->hash + Hash(" ") + r->hash + Hash(")");
        return;
    }
    if (type == "ABSTR") {
        assert(islower(l->type[0]));
        hash = Hash("(\\") + l->hash + Hash(".") + r->hash + Hash(")");
        return;
    } 
    if (type == "COND") 
        return;
    assert(false);
}

shared_ptr < Node > Node::getL() { return l; }

shared_ptr < Node > Node::getR() { return r; } 

//void Node::setL(shared_ptr < Node > v) {
    //l = v;
    //updateHash();
//}

//void Node::setR(shared_ptr < Node > v) {
    //r = v;
    //updateHash();
//}

string LexicalAnalyzer::curToken() {
    return token;
}

LambdaParser::LambdaParser(string s): lex(s) {
    lex.next();
}

shared_ptr < Node > LambdaParser::parseExp() {
    if (lex.curToken() == "\\") {
        lex.next();
        shared_ptr < Node > v = parseVar();
        assert(lex.curToken() == ".");
        lex.next();
        shared_ptr < Node > u = parseExp(); 
        return shared_ptr < Node > (new Node("ABSTR", v, u));
    }
    shared_ptr < Node > g = parseApply();
    if (lex.curToken() == "\\") {
        lex.next();
        shared_ptr < Node > v = parseVar();
        assert(lex.curToken() == ".");
        lex.next();
        shared_ptr < Node > u = parseExp(); 
        return shared_ptr < Node > (new Node("APPLY", g, SNode(new Node("ABSTR", v, u))));
    }
    return g;
}


shared_ptr < Node > LambdaParser::parseApply() {
    shared_ptr < Node > v = parseAtom();
    for (; lex.curToken() == "(" || isalpha(lex.curToken()[0]);) {
        shared_ptr < Node > u = parseAtom();
        v = shared_ptr < Node > (new Node("APPLY", v, u));
    }
    return v;
}

shared_ptr < Node > LambdaParser::parseAtom() {
    if (lex.curToken() == "(") {
        lex.next();
        shared_ptr < Node > v = parseExp();
        assert(lex.curToken() == ")");
        lex.next();
        return v;
    }
    return parseVar();
}


shared_ptr < Node > LambdaParser::parseVar() {
    assert(isalpha(lex.curToken()[0]));
    string s = lex.curToken();
    lex.next();
    return shared_ptr < Node > (new Node(s));
}

shared_ptr < Node > LambdaParser::parseCondition() {
    shared_ptr < Node > v = parseExp();    
    assert(lex.curToken() == "[");
    lex.next();
    shared_ptr < Node > u = parseVar();
    assert(lex.curToken() == ":=");
    lex.next();
    shared_ptr < Node > g = parseExp();
    assert(lex.curToken() == "]");
    lex.next();
    return shared_ptr < Node > (new Node("COND", v, shared_ptr < Node > (new Node("COND", u, g))));
}



// tools
set < string > genFV(shared_ptr < Node > v) {
    if (islower(v->type[0])) {
        set < string > q;
        q.insert(v->type);
        return q;
    }
    if (v->type == "APPLY") {
        auto r1 = genFV(v->getL());
        auto r2 = genFV(v->getR());
        r1.insert(r2.begin(), r2.end());
        return r1;
    }
    if (v->type == "ABSTR") {
        auto r = genFV(v->getR()); 
        r.erase(v->getL()->type);
        return r;
    }
    assert(false);
}

//bool checkFV(shared_ptr < Node > v, string var) {
    //if (islower(v->type[0]))
        //return v->type == var;
    //if (v->type == "APPLY") {
        //return checkFV(v->getL(), var) || checkFV(v->getR(), var);
    //}
    //if (v->type == "ABSTR") {
        //if (v->getL()->type == var) return 0;
        //return checkFV(v->getR(), var);
    //}
    //assert(false);
//}

int cntN = 0;

//shared_ptr < Node > createCopy(shared_ptr < Node > v) {
    //cntN++;
    //if (cntN % 500000 == 0)
        //db(cntN);
    //if (islower(v->type[0]))  
        //return shared_ptr < Node > (new Node(v->type));
    //return shared_ptr < Node > (new Node(v->type, createCopy(v->getL()), createCopy(v->getR())));
//}

void checkEqual(shared_ptr < Node > v, shared_ptr < Node > u) {
    assert(v != NULL && u != NULL);
    assert(v->type == u->type);
    if (v->getL() != NULL) checkEqual(v->getL(), u->getL());
    if (v->getL() != NULL) checkEqual(v->getR(), u->getR());

}

//int cntSub;

shared_ptr < Node > makeSubst(shared_ptr < Node > v, string name, shared_ptr < Node > u, int & cnt, FreeVarGenerator & gen) {
    //cntSub++;
    //if (cntSub % 1000000 == 0)
        //db(cntSub);

    if (islower(v->type[0])) {
        if (v->type == name) {
            cnt++;
            return u;
        }
        return v;
    }
    //db(v->type);
    if (v->type == "APPLY") {
        int cc = cnt;
        auto ll = makeSubst(v->getL(), name, u, cnt, gen);
        auto rr = makeSubst(v->getR(), name, u, cnt, gen);
        if (cnt > cc)  {
            v = shared_ptr < Node > (new Node(v->type, ll, rr));
        }
        return v;
    }
    if (v->type == "ABSTR") {
        if (v->getL()->type == name)
            return v;
        //auto fvU = genFV(u);
        //bool checkFVy = check

        if (!(islower(v->getL()->type[0]))) {
            db(v->getL()->type);
        }
        assert(islower(v->getL()->type[0]));
        string y = v->getL()->type;
        //if (checkFV(u, y)) {
        //if (false) {
        if (u->freeCh.count(y) == 1) {
            //assert(false);
            string z = gen.next();
            //v->getL()->type = z;
            int cc = 0;
            //shared_ptr < Node > gg;
            v = shared_ptr < Node > (new Node(v->type, shared_ptr < Node > (new Node(z)), makeSubst(v->getR(), y, shared_ptr < Node > (new Node(z)), cc, gen)));
            v = shared_ptr < Node > (new Node(v->type, v->getL(), makeSubst(v->getR(), name, u, cnt, gen)));
            //assert(cnt > 0);
            //db("b_2");
            //v = shared_ptr < Node > (new Node(v->type, ll, rr));
            //db("a_2");
        }
        else {
            int cc = cnt;
            auto rr = makeSubst(v->getR(), name, u, cnt, gen);
            if (cnt > cc) 
                v = shared_ptr < Node > (new Node(v->type, v->getL(), rr));
        }
        return v;
    }
    assert(false);
}

string genAns(shared_ptr < Node > v) {
    if (islower(v->type[0]))
        return v->type;
    if (v->type == "APPLY") return "(" + genAns(v->getL()) + " " + genAns(v->getR()) + ")";
    if (v->type == "ABSTR") return "(\\" + genAns(v->getL()) + "." + genAns(v->getR()) + ")";
    assert(false);
}


const int BASE = 37;
const char BACK_QUOTE = '`';

FreeVarGenerator::FreeVarGenerator(): cur(1) { }

void FreeVarGenerator::add(shared_ptr < Node > v) {
    if (islower(v->type[0])) 
        add(v->type);
    else {
        add(v->getL());
        add(v->getR());
    }
}

void FreeVarGenerator::add(string s) {
    q.insert(decode(s));
}

string FreeVarGenerator::code(int x) {
    string s; 
    for (; x > 0; x /= 10)
        s += '0' + x % 10; 
    reverse(s.begin(), s.end());
    return "t" + s;
}

int FreeVarGenerator::decode(string s) {
    if (s.size() > 5) return 0;
    if (s[0] != 't') return 0;
    int x = 0;
    for (int i = 1; i < (int)s.size(); i++) {
        x = x * 10 + s[i] - '0';
    }
    return x;
}

string FreeVarGenerator::next() {
    for (; q.count(cur) == 1; cur++);
    q.insert(cur);
    string s = code(cur);
    //db(s);
    return s;
}

shared_ptr < Node > parse(string s) {
    LambdaParser parser(s);
    return parser.parseExp();
}


