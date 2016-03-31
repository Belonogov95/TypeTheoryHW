#include "../LambdaParser/main.h"
#include "../LambdaParser/lambdaParser.h"


int cnt;
FreeVarGenerator gen;

map < ull, pair < Node *, int > > g;

const int MAX_LEN = 100;

Node * makeBRed(Node * v) {
    if (islower(v->type[0])) return v;
    if (v->type == "ABSTR") {
        //ull oldHash = v->getHash();
        v->setR(makeBRed(v->getR()));
        //if (cnt > 0 && v->getLen() < MAX_LEN) {
            //g[oldHash] = mp(createCopy(v), cnt);
        //}
        return v;
    }
    if (v->type == "APPLY") {
        if (v->getL()->type == "ABSTR") {
            vector < pair < ull, int > > forUpdate;
            for (;;) {
                if (g.count(v->getHash()) == 1) {
                    forUpdate.pb(mp(v->getHash(), cnt));
                    cnt += g[v->getHash()].sc;
                    v = g[v->getHash()].fr;
                }
                else
                    break;
            }
            for (auto x: forUpdate) {
                assert(cnt > x.sc);
                g[x.fr] = mp(v, cnt - x.sc);
            } 
            if (cnt > 0) {
                v = createCopy(v);
                return v;
            }

            int cc = 0;
            ull oldHash = v->getHash();
            v = makeSubst(v->getL()->getR(), v->getL()->getL()->type, v->getR(), cc, gen);
            g[oldHash] = mp(createCopy(v), 1);
            cnt = 1;
            return v;
        }
        ull oldHash = v->getHash();
        v->setL(makeBRed(v->getL()));
        if (cnt == 0) {
            v->setR(makeBRed(v->getR()));
        }
        if (cnt > 0 && v->getLen() < MAX_LEN) {
            g[oldHash] = mp(createCopy(v), cnt);
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
        
        //if (it % 10 == 0) {
            //db2(it, g.size());
            //db(genAns(head).size());
        //}
        cnt = 0;
        head = makeBRed(head);
        if (cnt > 1)
            db2(it, cnt);
        if (cnt == 0) {
            db2(it, g.size());
            string tmp = genAns(head);
            Node * tt = parse(tmp);
            //db(genAns(tt));
            cnt = 0;
            makeBRed(tt);
            assert(cnt == 0);
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

