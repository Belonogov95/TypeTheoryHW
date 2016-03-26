#include "main.h"
#include "lambdaParser.h"




LexicalAnalyzer::LexicalAnalyzer(string s): s(s), cur(0) { }

bool myCheck(char ch) {
    if ('a' <= ch && ch <= 'z') return 1;
    if ('0' <= ch && ch <= '9') return 1;
    if (ch == '`') return 1;
    return 0;
}

void LexicalAnalyzer::next() {
    db(cur);
    db(s.size());
    for (; cur < (int)s.size() && isspace(s[cur]); cur++);

    db(cur);
    if (cur == (int)s.size()) {
        token = "";
        return;
    } 

    if (s[cur] == '\\' || s[cur] == '.' || s[cur] == '(' || s[cur] == ')') {
        token = s.substr(cur, 1); 
        cur++;
        return;
    }
    assert(isalpha(s[cur]));
    int t = cur;
    for (; cur < (int)s.size() && myCheck(s[cur]); cur++);

    token = s.substr(t, cur - t);
}


Node :: Node (string type): type(type) { }

Node :: Node (string type, Node * ch1): type(type) {
    ch.pb(ch1);
}

Node :: Node (string type, Node * ch1, Node * ch2): type(type) {
    ch.pb(ch1);
    ch.pb(ch2);
}

Node :: Node (string type, Node * ch1, Node * ch2, Node * ch3): type(type) {
    ch.pb(ch1);
    ch.pb(ch2);
    ch.pb(ch3);
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
        return new Node("EXP", v, u);
    }
    Node * g = parseApply();
    if (lex.curToken() == "\\") {
        lex.next();
        Node * v = parseVar();
        assert(lex.curToken() == ".");
        lex.next();
        Node * u = parseExp(); 
        return new Node("EXP", g, v, u);
    }
    else {
        return new Node("EXP", g);
    }
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
    db2(lex.curToken().size(), lex.curToken());
    assert(isalpha(lex.curToken()[0]));
    string s = lex.curToken();
    lex.next();
    return new Node(s);
}


























