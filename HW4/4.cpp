#include "../LambdaParser/main.h"
#include "../LambdaParser/lambdaParser.h"


int cnt;
FreeVarGenerator gen;

map < ull, pair < shared_ptr < Node >, int > > g;

const int MAX_LEN = 10000000;

shared_ptr < Node > makeBRed(shared_ptr < Node > v) {
    if (islower(v->type[0])) return v;
    if (v->type == "ABSTR") {
        //ull oldHash = v->getHash();
        assert(cnt == 0);
        auto rr = makeBRed(v->getR());
        if (cnt > 0) {
            v = shared_ptr < Node > (new Node(v->type, v->getL(), rr));
        }
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
                return v;
            }
            int cc = 0;
            ull oldHash = v->getHash();

            //cerr << "================\t\t\tleft right: " << v->getL()->getLen() << " " << v->getR()->getLen();
            v = makeSubst(v->getL()->getR(), v->getL()->getL()->type, v->getR(), cc, gen);
            //cerr << "        " << cc << endl;

            if (v->getLen() < MAX_LEN)
                g[oldHash] = mp(v, 1);
            cnt = 1;
            return v;
        }
        ull oldHash = v->getHash();

        if (rand() % 10 == 0) {
            auto rr = makeBRed(v->getR());
            if (cnt > 0) {
                v = shared_ptr < Node > (new Node(v->type, v->getL(), rr)); 
            }
            else {
                auto ll = makeBRed(v->getL());
                if (cnt > 0) {
                    v = shared_ptr < Node > (new Node(v->type, ll, v->getR()));
                }
            }
        }
        else {
            auto ll = makeBRed(v->getL());
            if (cnt > 0) {
                v = shared_ptr < Node > (new Node(v->type, ll, v->getR()));
            }
            else {
                auto rr = makeBRed(v->getR());
                if (cnt > 0) {
                    v = shared_ptr < Node > (new Node(v->type, v->getL(), rr)); 
                }
            }
        }
        if (cnt > 0 && v->getLen() < MAX_LEN) {
            g[oldHash] = mp(v, cnt);
        }
        return v;
    }
    assert(false);

}

void solve() {
    string s;
    getline(cin, s); 
        
    shared_ptr < Node > head = parse(s);
    //exit(0);
    gen.add(head);

    int sumC = 0;
    for (int it = 0; ; it++) {
        if (it % 100 == 0) {
            db(it);
        }
        //if (it % 100 == 0) {
            //int sumLen = 0;
            //for (auto &x: g) {
                //sumLen += x.sc.fr->getLen();
                //assert(x.sc.fr->getLen() < MAX_LEN);
            //}
            ////cerr << "it g.size() len : " << it << " " << g.size() << " " << head->getLen() << "              sumC " << sumC << "           h " << head->h << endl;
            ////cerr << "con1 con2 dest: " << cntCon1 << " " << cntCon2 << " " << cntDest <<  "                cntSub: " << cntSub << endl;
        //}
        cnt = 0;
        head = makeBRed(head);
        sumC += cnt;
        //if (cnt > 1)
            //db2(it, cnt);
        if (cnt == 0) {
            db2(it, g.size());
            string tmp = genAns(head);
            shared_ptr < Node > tt = parse(tmp);
            //db(genAns(tt));
            cnt = 0;
            makeBRed(tt);
            assert(cnt == 0);
            db("OK");
            break;
        }
    }
    db(head->getLen());
    cout << genAns(head) << endl;

}

int main() {
    freopen("task4.in", "r", stdin);
    //freopen("task4.out", "w", stdout);
    solve();

    //cerr << gen.next() << endl;
    //cerr << gen.next() << endl;

    return 0;
}

