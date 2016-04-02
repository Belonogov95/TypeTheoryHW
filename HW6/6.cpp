#include "../LambdaParser/main.h"
#include "../LambdaParser/lambdaParser.h"

struct TNode {
    int id;
    TNode * l, * r;
    TNode(int id): id(id), l(NULL), r(NULL) { }
    TNode(TNode * l, TNode * r): id(-1), l(l), r(r) { }
    string toString() {
        if (id >= 0) 
            return "t" + to_string(id);
        return "(" + l->toString() + "->" + r->toString() + ")";
    }
};

int cur = 0;
map < string, int > varId; 
map < int, TNode * > g;


void ret() {
    cout << "Lambda expression doesn't has a type" << endl;
    exit(0);
}

bool checkEqual(TNode * v, TNode * u) {
    if ((v == NULL) != (u != NULL)) return 0;
    assert(v != NULL && u != NULL);
    if (v->id != u->id) return 0;
    if (v->l != NULL && !checkEqual(v->l, u->l))
        return 0;
    if (v->r != NULL && !checkEqual(v->r, u->r))
        return 0;

    return 1;
}

bool checkFree(TNode * v, int id) {
    if (v == NULL) return 0;
    if (v->id == id) return 1;
    return checkFree(v->l, id) || checkFree(v->r, id);
}

TNode * makeSubst(TNode * v, int id, TNode * u) {
    if (v->id == id) return u;
    if (v->l != NULL)
        v->l = makeSubst(v->l, id, u);
    if (v->r != NULL)
        v->r = makeSubst(v->r, id, u);
    return v;
}

void add(TNode * v, TNode * u) {
    //db2(v->id, u->id);
    if (v->id == -1 && u-> id == -1) {
        add(v->l, u->l);
        add(v->r, u->r);
        return;
    }
    if (v->id < u->id) swap(v, u);
    int id = v->id;
    assert(id >= 0);

    if (checkFree(u, id))
        ret();
    
    if (g.count(id) == 1) {
        add(g[id], u);
        return;
    }  
    for (auto & x: g) {
        if (checkFree(x.sc, id)) {
            if (checkFree(u, x.fr)) {
                ret();
            } 
            x.sc = makeSubst(x.sc, id, u);
        } 
    }
    for (auto &x: g) {
        if (checkFree(u, x.fr)) {
            u = makeSubst(u, x.fr, x.sc);
        }
    }
    g[id] = u;
}

int rec(shared_ptr < Node > v) {
    if (islower(v->type[0])) {
        int id;
        if (varId.count(v->type) == 0) 
            varId[v->type] = cur++;
        id = varId[v->type]; 
        return id; 
    } 
    int l = rec(v->l);
    int r = rec(v->r);
    int id = cur++;
    //db2(l, r);
    if (v->type == "ABSTR") {
        TNode * tv = new TNode (new TNode(l), new TNode(r));
        add(new TNode(id), tv);
        return id;
    }
    if (v->type == "APPLY") {
        TNode * tv = new TNode(new TNode(r), new TNode(id));
        add(new TNode(l), tv);
        return id;
    }
    assert(false);

} 

string myToString (int x) {
    char s[100];
    sprintf(s, "%d", x);
    return s;
}

string f(int id) {
    if (g.count(id) == 1) {
        return g[id]->toString();
    }
    return "t" + myToString(id);
}

void solve() {
    string s;
    getline(cin, s);
    shared_ptr < Node > head = parse(s);

    int id = rec(head);

    cout << g[id]->toString() << endl;
    for (auto x: varId) {
        cout << x.fr << ":" << f(x.sc) << endl;
    }


    
}

int main() {
    freopen("task6.in", "r", stdin);

    solve();

    return 0;
}


