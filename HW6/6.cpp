#include "../LambdaParser/main.h"
#include "../LambdaParser/lambdaParser.h"
#include "termParser.h"

int cur = 1;
map < string, string > varId; 
vector < pair < shared_ptr < TNode > , shared_ptr < TNode > > > G;

void ret() {
    cout << "Lambda expression doesn't has a type" << endl;
    exit(0);
}


//bool checkEqual(shared_ptr < Node > v, shared_ptr < Node > u) {
    //if ((v == NULL) != (u != NULL)) return 0;
    //assert(v != NULL && u != NULL);
    //if (v->id != u->id) return 0;
    //if (v->l != NULL && !checkEqual(v->l, u->l))
        //return 0;
    //if (v->r != NULL && !checkEqual(v->r, u->r))
        //return 0;

    //return 1;
//T}

//bool checkFree(shared_ptr < TNode > v, int id) {
    //if (v == NULL) return 0;
    //if (v->id == id) return 1;
    //return checkFree(v->l, id) || checkFree(v->r, id);
//}

string myToString(int id) {
    char s[100];
    sprintf(s, "%d", id);
    return s;
}

bool findVar(string var, shared_ptr < TNode > u) {
    if (u->type == var) return 1; 
    for (auto h: u->ch)
        if (findVar(var, h)) return 1;        
    return 0;
}

shared_ptr < TNode > makeSubst(shared_ptr < TNode > v, string var, shared_ptr < TNode > u) {
    //cerr << "---------------   " <<  v << " " << var << " " << u << endl;
    if (v->type == var) return u;
    vector < shared_ptr < TNode > > ch;
    for (auto & h: v->ch) {
        ch.pb(makeSubst(h, var, u));
    }
    return shared_ptr < TNode > (new TNode(v->type, ch));
}

string printAns(shared_ptr < TNode > v) {
    if (v->checkVar()) {
        return v->type;         
    }
    return "(" + printAns(v->ch[0]) + "->" + printAns(v->ch[1]) + ")";  
}

void go() {
    bool flagChanged = 1;
    for (int it = 0;flagChanged; it++) {
        //cerr << "============================\n";
        //for (auto x: G)
            //cerr << printAns(x.fr) << " = " << printAns(x.sc) << endl;
        //cerr << "============================\n";

        
        flagChanged = 0;
        for (int i = 0; i < (int)G.size(); i++) {
            if (checkEqual(G[i].fr, G[i].sc)) {
                G.erase(G.begin() + i);
                flagChanged = 1;
                break;
            }
        } 
        if (flagChanged) continue;

        for (int i = 0; i < (int)G.size(); i++) {
            shared_ptr < TNode > v = G[i].fr;
            shared_ptr < TNode > u = G[i].sc;
            if (v->checkFun() && u->checkFun()) {
                if (v->type != u->type || v->ch.size() != u->ch.size()) {
                    cerr << v << " != " << u << endl;
                    cerr << "fail\n";
                    exit(0);
                }
                G.erase(G.begin() + i);
                for (int j = 0; j < (int)v->ch.size(); j++) {
                    G.pb(mp(v->ch[j], u->ch[j]));
                }
                flagChanged = 1;
                break;
            }
        }  

        if (flagChanged) continue;

        for (int i = 0; i < (int)G.size(); i++) {
            if (G[i].fr->checkFun() && G[i].sc->checkVar()) {
                swap(G[i].fr, G[i].sc);  
            }
        }

        // check x = f(x, ... );

        for (int i = 0; i < (int)G.size(); i++) {
            if (G[i].fr->checkVar() && G[i].sc->checkFun()) {
                if (findVar(G[i].fr->type, G[i].sc)) {
                    cerr << endl;
                    cerr << G[i].fr << " = " << G[i].sc << endl;
                    cerr << "fail\n";
                    exit(0);
                }                    
            }
        }

        //cerr << "last part====================\n";
        //db2(G.back().fr->checkVar(), G.back().sc->checkFun());
        for (int i = 0; i < (int)G.size(); i++) 
            //if (G[i].fr->checkVar() && G[i].sc->checkFun()) {
            if (G[i].fr->checkVar()) {
                for (int j = 0; j < (int)G.size(); j++) {
                    if (i == j) continue;
                    //db2(i, j);
                    if (findVar(G[i].fr->type, G[j].fr)) {
                        G[j].fr = makeSubst(G[j].fr, G[i].fr->type, G[i].sc);
                        flagChanged = 1;
                    }
                    if (findVar(G[i].fr->type, G[j].sc)) {
                        G[j].sc = makeSubst(G[j].sc, G[i].fr->type, G[i].sc);
                        flagChanged = 1;
                    }
                }
            }
    }
}


string rec(shared_ptr < Node > v, map < string, string > q) {
    //cerr << genAns(v) << endl;
    if (v->isVar()) {
        //int id;
        //db(q.size());
        //for (auto x: q)
            //db2(x.fr, x.sc);
        //db(v->type);

        assert(q.count(v->type) == 1);
        return q[v->type]; 
    } 

    //db(v->type);
    if (v->isAbstr()) {
        string s = v->l->type;
        string vv = "t" + myToString(cur++);
        q[s] = vv; 
    }

    string l = rec(v->l, q);
    string r = rec(v->r, q);
    string myId = "t" + myToString(cur++);
    varId[myId] = genAns(v);

    //cerr << "l r myId: " << l << " " << r << " " << myId << endl;
    if (v->type == "APPLY") {
        shared_ptr < TNode > ll(new TNode(l));
        shared_ptr < TNode > rr(new TNode(r));
        shared_ptr < TNode > me(new TNode(myId));
        shared_ptr < TNode > res(new TNode("a", {rr, me}));
        G.pb(mp(ll, res));
        return myId;
    }
    if (v->type == "ABSTR") {
        shared_ptr < TNode > ll(new TNode(l));
        shared_ptr < TNode > rr(new TNode(r));
        shared_ptr < TNode > res(new TNode("a", {ll, rr}));
        shared_ptr < TNode > me(new TNode(myId));
        G.pb(mp(me, res));
        return myId;
    }
    assert(false);
}



void solve() {
    string s;
    getline(cin, s);
    shared_ptr < Node > head = parse(s);
    map < string, string > q;
    string answer = rec(head, q);
    //cerr << genAns(head) << endl;
    //for (auto x: G) {
        //cerr << x.fr << " =  " << x.sc << endl;
    //}
    //for (auto x: varId) {
        //cerr << x.fr << ": " << x.sc << endl;
    //}

    go();
    //db(answer);
    //for (auto x: G) {
        //cerr << x.fr << " =  " << x.sc << endl;
    //}
    
    for (int i = 0; i < (int)G.size(); i++) {
        if (G[i].fr->type == answer) {
            cerr << "result:   " << printAns(G[i].sc) << endl;
        }
    }

    //cout << g[id]->toString() << endl;
    for (auto x: varId) {
        cout << x.fr << ":" << x.sc << endl;
    }
}

int main() {
    freopen("task6.in", "r", stdin);


    solve();

    return 0;
}


