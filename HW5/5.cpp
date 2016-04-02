#include "main.h"
#include "termParser.h"


vector < pair < shared_ptr < TNode >, shared_ptr < TNode > > > G;



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

void go() {
    bool flagChanged = 1;
    for (int it = 0;flagChanged; it++) {
        //db(it);
        
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
                    cerr << G[i].fr << " = " << G[i].sc << endl;
                    cerr << "fail\n";
                    exit(0);
                }                    
            }
        }

        for (int i = 0; i < (int)G.size(); i++) 
            if (G[i].fr->checkVar() && G[i].sc->checkFun()) {
                //db(i);
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

void solve() {
    string s;
    for (;getline(cin, s);) {
        bool flag = 1;
        for (auto ch: s)
            flag &= isspace(ch);
        if (flag) continue;
        
        TermParser parser(s);
        shared_ptr < TNode > head = parser.parseEquation();
        assert(head->ch.size() == 2);
        shared_ptr < TNode > v = head->ch[0]; 
        shared_ptr < TNode > u = head->ch[1];
        G.pb(mp(v, u));
    }
    go(); 

    for (auto x: G) {
        cout << x.fr << "=" << x.sc << endl;
    }
}


int main() {
    freopen("task5.in", "r", stdin);
    //freopen("task5.out", "w", stdout);

    solve();

    return 0;
}



