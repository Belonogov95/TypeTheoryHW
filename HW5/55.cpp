#include "main.h"
#include "termParser.h"


vector < pair < string, Node * > > g;



bool findVar(string var, Node * u) {
    if (u->type == var) return 1; 
    for (auto h: u->ch)
        if (findVar(var, h)) return 1;        
    return 0;
}

Node * makeSubst(Node * v, string var, Node * u) {
    if (v->type == var) return u;
    for (auto & h: v->ch) {
        h = makeSubst(h, var, u);
    }
    return v;
}

void add(Node * v, Node * u) {
    if (checkEqual(v, u)) return;
    if (v->checkFun() && u->checkFun()) {
        if (v->type != u->type || v->ch.size() != u->ch.size()) {
            cout << v << " and " << u << " can not be equal\n";
            exit(0);
        }
        for (int i = 0; i < (int)v->ch.size(); i++) {
            add(v->ch[i], u->ch[i]);
        }
        return;
    } 
    if (v->checkFun() && u->checkVar()) 
        swap(v, u);
    string var = v->type;
    assert(v->checkVar());

    for (auto x: g) {
        if (x.fr == var) {
            add(x.sc, u);
            return;
            //if (checkEqual(x.sc, u)) return;
            //cout << var + "=" << u << endl;
            //cout << var + "=" << x.sc << endl;
            //cout << "fail\n";
            //exit(0);
        }
    } 

    if (findVar(var, u)) {
        cout << "fail\n";
        cout << var << " has occurance in " << u << endl;
        exit(0);
    }
    for (auto & x: g) {
        if (findVar(var, x.sc)) {
            if (findVar(x.fr, u))  {
                cout << "fail\n";
                cout << x.fr << " has occurance in " << u << endl;
                exit(0);
            }
            x.sc = makeSubst(x.sc, var, u);
        }
    }
    g.pb(mp(var, u));
}

void solve() {
    string s;
    for (;getline(cin, s);) {
        bool flag = 1;
        for (auto ch: s)
            flag &= isspace(ch);
        if (flag) continue;
        
        TermParser parser(s);
        Node * head = parser.parseEquation();
        assert(head->ch.size() == 2);
        Node * v = head->ch[0]; 
        Node * u = head->ch[1];
        //db("before add");
        add(v, u);
    }

    for (auto x: g) {
        cout << x.fr << "=" << x.sc << endl;
    }
}


int main() {
    freopen("task5.in", "r", stdin);
    //freopen("task5.out", "w", stdout);

    solve();

    return 0;
}




