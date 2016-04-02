#include "termParser.h"



TNode::TNode (string type, vector < shared_ptr < TNode > > ch): type(type), ch(ch) { }

TNode::TNode (string type): type(type) { }


bool TNode::checkFun() {
    return isFun(type[0]);
}

bool TNode::checkVar() {
    return isVar(type[0]);
}

bool checkEqual(shared_ptr < TNode > v, shared_ptr < TNode > u) {
    if (v->type != u->type || v->ch.size() != u->ch.size()) return 0;
    for (int i = 0; i < (int)v->ch.size(); i++)
        if (!checkEqual(v->ch[i], u->ch[i])) return 0;
    return 1;
}

//void TNode::printTerm() {
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

ostream & operator <<(ostream & os, shared_ptr < TNode > v) {
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


LexicalAnalyzerT::LexicalAnalyzerT(string s): s(s) {
    cur = 0;
    next();
}

bool myCheck2(char ch) {
    return ('a' <= ch && ch <= 'z') || ('0' <= ch && ch <= '9') || ch == '`';
}

bool isVar(char ch) {
    return 'i' <= ch && ch <= 'z';
}

bool isFun(char ch) {
    return 'a' <= ch && ch <= 'h';
}


void LexicalAnalyzerT::next() {
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
    for (; cur < (int)s.size() && myCheck2(s[cur]); cur++);

    token = s.substr(t, cur - t);
    
}

string LexicalAnalyzerT::curToken() {
    return token;
}

TermParser::TermParser(string s): lex(s) { }
shared_ptr < TNode > TermParser::parseEquation() {
    //db(lex.curToken());
    shared_ptr < TNode > v = parseTerm();
    //db(lex.curToken());
    assert(lex.curToken() == "=");
    lex.next();
    //db(lex.curToken());
    shared_ptr < TNode > u = parseTerm();
    return shared_ptr < TNode > (new TNode("E", vector < shared_ptr < TNode > > {v, u}));
}

shared_ptr < TNode > TermParser::parseTerm() {
    if (isVar(lex.curToken()[0])) {
        return parseVar();
    }
    //db(lex.curToken());
    assert(isFun(lex.curToken()[0])); 
    return parseFun();
}

shared_ptr < TNode > TermParser::parseFun() {
    string s = lex.curToken();
    //db(lex.curToken());
    lex.next();
    //db(lex.curToken());
    assert(lex.curToken() == "(");    
    lex.next();
    vector < shared_ptr < TNode > > ch;
    ch.pb(parseTerm());
    for (; lex.curToken() == ","; ) {
        lex.next();
        ch.pb(parseTerm());
    }
    assert(lex.curToken() == ")");
    lex.next();
    return shared_ptr < TNode > (new TNode(s, ch));
}

shared_ptr < TNode > TermParser::parseVar() {
    string s = lex.curToken();
    lex.next();
    return shared_ptr < TNode > (new TNode(s, vector < shared_ptr < TNode > >()));
}




