#include "termParser.h"


Node::Node (string type, vector < shared_ptr < Node > > ch): type(type), ch(ch) { }


bool Node::checkFun() {
    return isFun(type[0]);
}

bool Node::checkVar() {
    return isVar(type[0]);
}

bool checkEqual(shared_ptr < Node > v, shared_ptr < Node > u) {
    if (v->type != u->type || v->ch.size() != u->ch.size()) return 0;
    for (int i = 0; i < (int)v->ch.size(); i++)
        if (!checkEqual(v->ch[i], u->ch[i])) return 0;
    return 1;
}

//void Node::printTerm() {
    //if (isFun(type[0])) {
        //cout << type << "(";
        //ch[0]->printTerm();
        //for (int i = 1; i < (int)ch.size(); i++) {
            //cout << ",";
            //ch[i]->printTerm();
        //}
        //cout << ")";
    //}
    //else {
        //assert(isVar(type[0]));
        //cout << type;
    //}
//}

ostream & operator <<(ostream & os, shared_ptr < Node > v) {
    if (isFun(v->type[0])) {
        os << v->type << "(" << v->ch[0];
        for (int i = 1; i < (int)v->ch.size(); i++) {
            os << "," << v->ch[i];
        }
        os << ")";
    }
    else {
        assert(isVar(v->type[0]));
        os << v->type;
    }
    return os;
}


LexicalAnalyzer::LexicalAnalyzer(string s): s(s) {
    cur = 0;
    next();
}

bool myCheck(char ch) {
    return ('a' <= ch && ch <= 'z') || ('0' <= ch && ch <= '9') || ch == '`';
}

bool isVar(char ch) {
    return 'i' <= ch && ch <= 'z';
}

bool isFun(char ch) {
    return 'a' <= ch && ch <= 'h';
}


void LexicalAnalyzer::next() {
    for (; cur < (int)s.size() && isspace(s[cur]); cur++);
    if (cur == (int)s.size()) {
        token = "";
        return;
    } 
    if (s[cur] == '(' || s[cur] == ')' || s[cur] == ',' || s[cur] == '=') {
        token = s[cur];
        cur++;
        return;
    }
    assert(islower(s[cur]));    
    int t = cur;
    for (; cur < (int)s.size() && myCheck(s[cur]); cur++);

    token = s.substr(t, cur - t);
    
}

string LexicalAnalyzer::curToken() {
    return token;
}

TermParser::TermParser(string s): lex(s) { }
shared_ptr < Node > TermParser::parseEquation() {
    //db(lex.curToken());
    shared_ptr < Node > v = parseTerm();
    //db(lex.curToken());
    assert(lex.curToken() == "=");
    lex.next();
    //db(lex.curToken());
    shared_ptr < Node > u = parseTerm();
    return shared_ptr < Node > (new Node("E", vector < shared_ptr < Node > > {v, u}));
}

shared_ptr < Node > TermParser::parseTerm() {
    if (isVar(lex.curToken()[0])) {
        return parseVar();
    }
    //db(lex.curToken());
    assert(isFun(lex.curToken()[0])); 
    return parseFun();
}

shared_ptr < Node > TermParser::parseFun() {
    string s = lex.curToken();
    //db(lex.curToken());
    lex.next();
    //db(lex.curToken());
    assert(lex.curToken() == "(");    
    lex.next();
    vector < shared_ptr < Node > > ch;
    ch.pb(parseTerm());
    for (; lex.curToken() == ","; ) {
        lex.next();
        ch.pb(parseTerm());
    }
    assert(lex.curToken() == ")");
    lex.next();
    return shared_ptr < Node > (new Node(s, ch));
}

shared_ptr < Node > TermParser::parseVar() {
    string s = lex.curToken();
    lex.next();
    return shared_ptr < Node > (new Node(s, vector < shared_ptr < Node > >()));
}




