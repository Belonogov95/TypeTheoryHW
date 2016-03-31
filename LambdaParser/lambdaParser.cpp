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


Node :: Node (string type): type(type), l(NULL), r(NULL), hash(type) {
}

Node :: Node (string type, Node * l, Node * r): type(type), l(l), r(r) { 
    updateHash();
}

ull Node::getHash() {
    return hash.hash;
}

int Node::getLen() {
    return hash.len;
}

void Node::updateHash() {
    if (type == "APPLY") {
        hash = Hash("(") + l->hash + Hash(" ") + r->hash + Hash(")");
        return;
    }
    if (type == "ABSTR") {
        hash = Hash("(\\") + l->hash + Hash(".") + r->hash + Hash(")");
        return;
    } 
    if (type == "COND") 
        return;
    assert(false);
}

Node * Node::getL() { return l; }

Node * Node::getR() { return r; } 

void Node::setL(Node * v) {
    l = v;
    updateHash();
}

void Node::setR(Node * v) {
    r = v;
    updateHash();
}

string LexicalAnalyzer::curToken() {
    return token;
}

LambdaParser::LambdaParser(string s): lex(s) {
    lex.next();
}

Node * LambdaParser::parseExp() {
    if (lex.curToken() == "\\") {
        lex.next();
        Node * v = parseVar();
        assert(lex.curToken() == ".");
        lex.next();
        Node * u = parseExp(); 
        return new Node("ABSTR", v, u);
    }
    Node * g = parseApply();
    if (lex.curToken() == "\\") {
        lex.next();
        Node * v = parseVar();
        assert(lex.curToken() == ".");
        lex.next();
        Node * u = parseExp(); 
        return new Node("APPLY", g, new Node("ABSTR", v, u));
    }
    return g;
}


Node * LambdaParser::parseApply() {
    Node * v = parseAtom();
    for (; lex.curToken() == "(" || isalpha(lex.curToken()[0]);) {
        Node * u = parseAtom();
        v = new Node("APPLY", v, u);
    }
    return v;
}

Node * LambdaParser::parseAtom() {
    if (lex.curToken() == "(") {
        lex.next();
        Node * v = parseExp();
        assert(lex.curToken() == ")");
        lex.next();
        return v;
    }
    return parseVar();
}


Node * LambdaParser::parseVar() {
    assert(isalpha(lex.curToken()[0]));
    string s = lex.curToken();
    lex.next();
    return new Node(s);
}

Node * LambdaParser::parseCondition() {
    Node * v = parseExp();    
    assert(lex.curToken() == "[");
    lex.next();
    Node * u = parseVar();
    assert(lex.curToken() == ":=");
    lex.next();
    Node * g = parseExp();
    assert(lex.curToken() == "]");
    lex.next();
    return new Node("COND", v, new Node("COND", u, g));
}



// tools
//set < string > genFV(Node * v) {
    //if (islower(v->type[0])) {
        //set < string > q;
        //q.insert(v->type);
        //return q;
    //}
    //if (v->type == "APPLY") {
        //auto r1 = genFV(v->getL());
        //auto r2 = genFV(v->getR());
        //r1.insert(r2.begin(), r2.end());
        //return r1;
    //}
    //if (v->type == "ABSTR") {
        //auto r = genFV(v->getR()); 
        //r.erase(v->getL()->type);
        //return r;
    //}
    //assert(false);
//}

bool checkFV(Node * v, string var) {
    if (islower(v->type[0]))
        return v->type == var;
    if (v->type == "APPLY") {
        return checkFV(v->getL(), var) || checkFV(v->getR(), var);
    }
    if (v->type == "ABSTR") {
        if (v->getL()->type == var) return 0;
        return checkFV(v->getR(), var);
    }
    assert(false);
}

int cntN = 0;

Node * createCopy(Node * v) {
    cntN++;
    if (cntN % 500000 == 0)
        db(cntN);
    if (islower(v->type[0]))  
        return new Node(v->type);
    return new Node(v->type, createCopy(v->getL()), createCopy(v->getR()));
}

void checkEqual(Node * v, Node * u) {
    assert(v != NULL && u != NULL);
    assert(v->type == u->type);
    if (v->getL() != NULL) checkEqual(v->getL(), u->getL());
    if (v->getL() != NULL) checkEqual(v->getR(), u->getR());

}

Node * makeSubst(Node * v, string name, Node * u, int & cnt, FreeVarGenerator & gen) {
    if (islower(v->type[0])) {
        if (v->type == name) {
            cnt++;
            return createCopy(u);
        }
        return v;
    }
    //db(v->type);
    if (v->type == "APPLY") {
        v->setL(makeSubst(v->getL(), name, u, cnt, gen));
        v->setR(makeSubst(v->getR(), name, u, cnt, gen));
        return v;
    }
    if (v->type == "ABSTR") {
        if (v->getL()->type == name)
            return v;
        //auto fvU = genFV(u);
        //bool checkFVy = check

        assert(islower(v->getL()->type[0]));
        string y = v->getL()->type;
        if (checkFV(u, y)) {
            //assert(false);
            string z = gen.next();
            v->getL()->type = z;
            int cc = 0;
            //Node * gg;
            v->setR(makeSubst(v->getR(), y, new Node(z), cc, gen));
            v->setR(makeSubst(v->getR(), name, u, cnt, gen));
        }
        else  
            v->setR(makeSubst(v->getR(), name, u, cnt, gen));
        return v;
    }
    assert(false);
}

string genAns(Node * v) {
    if (islower(v->type[0]))
        return v->type;
    if (v->type == "APPLY") return "(" + genAns(v->getL()) + " " + genAns(v->getR()) + ")";
    if (v->type == "ABSTR") return "(\\" + genAns(v->getL()) + "." + genAns(v->getR()) + ")";
    assert(false);
}


const int BASE = 37;
const char BACK_QUOTE = '`';

FreeVarGenerator::FreeVarGenerator(): cur(0) { }

void FreeVarGenerator::add(Node * v) {
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
    if (x == 0) return "a";
    for (; x > 0; x /= BASE) {
        char ch = x % BASE;
        if (ch < 26)
            ch += 'a';
        else if (ch < 36)
            ch += '0';
        else
            ch = BACK_QUOTE;
        s += ch;
    } 

    reverse(s.begin(), s.end());
    return s;
}

int FreeVarGenerator::decode(string s) {
    if (s.size() > 5) return int(1e9);
    int h = 0;
    int deg = 1;
    for (int i = 0; i < (int)s.size(); i++) {
        int ch;
        if (islower(s[i])) 
            ch = s[i] - 'a';
        else if (isdigit(s[i])) 
            ch = s[i] - '0';
        else {
            assert(s[i] == BACK_QUOTE);
            ch = BASE - 1;
        }
        assert(0 <= ch && ch < BASE);
        h += deg * ch;                
        deg *= BASE;
    }
    return h;
}

string FreeVarGenerator::next() {
    for (; q.count(cur) == 1; cur++);
    q.insert(cur);
    return code(cur);
}

Node * parse(string s) {
    LambdaParser parser(s);
    return parser.parseExp();
}


