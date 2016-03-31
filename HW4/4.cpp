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
    //db(v->type);
    if (islower(v->type[0])) return v;
    if (v->type == "ABSTR") {
        v->setR(makeBRed(v->getR()));
        return v;
    }
    if (v->type == "APPLY") {
        if (v->getL()->type == "ABSTR") {
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
            v = makeSubst(v->getL()->getR(), v->getL()->getL()->type, v->getR(), cnt, gen);
            Node * tmp = createCopy(v);
            g[oldHash] = tmp;
            flag = 1;
            return v;
        }
        ull oldHash = v->getHash();
        v->setL(makeBRed(v->getL()));
        if (!flag) {
            v->setR(makeBRed(v->getR()));
        }
        if (flag) {
            g[oldHash] = createCopy(v);
        }

        return v;
    }
    assert(false);

}

void solve() {
    string s;
    getline(cin, s); 
        
    Node * head = parse(s);
    //exit(0);
    gen.add(head);

    for (int it = 0; ; it++) {
        db2(it, g.size());
        if (it % 1000 == 0) {
            db2(it, g.size());
        }
        flag = 0;
        head = makeBRed(head);
        if (!flag) {
            db(it);
            string tmp = genAns(head);
            Node * tt = parse(tmp);
            //db(genAns(tt));
            flag = 0;
            makeBRed(tt);
            assert(!flag);
            db("OK");
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

