#include "../LambdaParser/main.h"
#include "../LambdaParser/lambdaParser.h"


bool flag;
FreeVarGenerator gen;

map < ull, Node * > g;

//ull getHash(string s) {
    //ull res = 0;
    //for (auto ch: s)
        //res = res * P + ch;
    //return res;
//}

Node * makeBRed(Node * v) {
    if (islower(v->type[0])) return v;
    if (v->type == "ABSTR") {
        v->r = makeBRed(v->r);
        return v;
    }
    if (v->type == "APPLY") {
        if (v->l->type == "ABSTR") {
            vector < ull > forUpdate;
            for (;;) {
                if (g.count(v->getHash()) == 1) {
                    flag = 1; 
                    forUpdate.pb(v->getHash());
                    v = g[v->getHash()];
                }
                else
                    break;
            }
            for (auto x: forUpdate) {
                g[x] = v;
            } 
            if (flag) {
                v = createCopy(v);
                return v;
            }

            int cnt = 0;
            ull oldHash = v->getHash();
            v = makeSubst(v->l->r, v->l->l->type, v->r, cnt, gen);
            Node * tmp = createCopy(v);
            g[oldHash] = tmp;
            flag = 1;
            return v;
        }
        v->l = makeBRed(v->l);
        if (flag) return v;
        v->r = makeBRed(v->r);
        return v;
    }
    assert(false);

}

void solve() {
    string s;
    getline(cin, s); 
        
    Node * head = parse(s);
    //db2(getHash(genAns(head)), head->hash.hash);
    //exit(0);
    gen.add(head);

    for (int it = 0; it < 30 ; it++) {
        db(it);
            //db(clock() * 1.0 / CLOCKS_PER_SEC);
            //db(it);
            //db(genAns(head).size());
            ////db(genAns(head));
            //db(gen.cur);
        flag = 0;
        head = makeBRed(head);
        cerr << genAns(head) << endl;
        if (!flag) {
            db(it);
            break;
        }
    }
    cout << genAns(head) << endl;

}

int main() {
    freopen("task4.in", "r", stdin);
    //freopen("task4.out", "w", stdout);
    solve();

    return 0;
}

