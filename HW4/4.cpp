#include "../LambdaParser/main.h"
#include "../LambdaParser/lambdaParser.h"


int cnt;
FreeVarGenerator gen;

map < ull, shared_ptr < Node > > normMem, headMem;

const int MAX_LEN = 10000000;
int cntDone;


shared_ptr < Node > normalForm(shared_ptr < Node > v);

shared_ptr < Node > headForm(shared_ptr < Node > v) {
    if (v->isVar() || v->isAbstr()) {
        return v;
    }
    if (headMem.count(v->getHash()) == 1) return headMem[v->getHash()];
    
    ull oldHash = v->getHash();
    if (v->isApply()) {
        v = shared_ptr < Node > (new Node(v->type, headForm(v->getL()), v->getR()));
        if (v->getL()->isAbstr()) {
            int cc = 0;
            v = makeSubst(v->getL()->getR(), v->getL()->getL()->type, v->getR(), cc, gen);
            v = headForm(v);
        }
        else {
            v = shared_ptr < Node > (new Node(v->type, v->getL(), normalForm(v->getR())));
        }
    } 
    assert(v->getHash() != oldHash);
    headMem[oldHash] = v;
    return v;
}

shared_ptr < Node > normalForm(shared_ptr < Node > v) {
    if (v->isVar()) return v;
    //db2(normMem.size(), headMem.size());

    if (normMem.count(v->getHash() == 1)) return normMem[v->getHash()];

    ull oldHash = v->getHash();

    if (v->isAbstr()) {
        v = shared_ptr < Node > (new Node(v->type, v->getL(), normalForm(v->getR()))); 
    }
    if (v->isApply()) {
        v = shared_ptr < Node > (new Node(v->type, headForm(v->getL()), v->getR()));
        if (!v->getL()->isAbstr()) {
            v = shared_ptr < Node > (new Node(v->type, v->getL(), normalForm(v->getR())));
        }
        else {
            int cc = 0;
            v = makeSubst(v->getL()->getR(), v->getL()->getL()->type, v->getR(), cc, gen);
            v = normalForm(v);
        }
    }
    normMem[oldHash] = v;
    return v;
}

//shared_ptr < Node > makeBRed(shared_ptr < Node > v) {
    //if (islower(v->type[0])) return mp(v, 0);
    //if (v->type == "ABSTR") {
        ////ull oldHash = v->getHash();
        //assert(cnt == 0);
        //auto rr = makeBRed(v->getR()).fr;
        //if (cnt > 0) {
            //v = shared_ptr < Node > (new Node(v->type, v->getL(), rr));
        //}
        ////if (cnt > 0 && v->getLen() < MAX_LEN) {
            ////g[oldHash] = mp(createCopy(v), cnt);
        ////}
        //return mp(v, 0);
    //}

    //if (v->type == "APPLY") {
        //if (v->getL()->type != "ABSTR") {
            //ull oldHash = v->getHash();

            //auto ll = makeBRed(v->getL()).fr;
            //if (cnt > 0) {
                //v = shared_ptr < Node > (new Node(v->type, ll, v->getR()));
            //}
            //else {
                //auto rr = makeBRed(v->getR()).fr;
                //if (cnt > 0) {
                    //v = shared_ptr < Node > (new Node(v->type, v->getL(), rr)); 
                //}
            //}
            //if (cnt > 0 && v->getLen() < MAX_LEN) {
                //g[oldHash] = mp(v, cnt);
            //}
            //return v;
        //}
        //if (v->getL()->type == "ABSTR") {
            //vector < pair < ull, int > > forUpdate;
            //for (;;) {
                //if (g.count(v->getHash()) == 1) {
                    //forUpdate.pb(mp(v->getHash(), cnt));
                    //cnt += g[v->getHash()].sc;
                    //v = g[v->getHash()].fr;
                //}
                //else
                    //break;
            //}
            //for (auto x: forUpdate) {
                //assert(cnt > x.sc);
                //g[x.fr] = mp(v, cnt - x.sc);
            //} 
            //if (cnt > 0) {
                //return mp(v, 1);
            //}
            //int cc = 0;
            //ull oldHash = v->getHash();

            ////cerr << "================\t\t\tleft right: " << v->getL()->getLen() << " " << v->getR()->getLen();
            //v = makeSubst(v->getL()->getR(), v->getL()->getL()->type, v->getR(), cc, gen);
            ////cerr << "        " << cc << endl;

            //if (v->getLen() < MAX_LEN)
                //g[oldHash] = mp(v, 1);
            //cnt = 1;
            //return mp(v, 1);
        //}
    //}
    //assert(false);

//}

void solve() {
    string s;
    getline(cin, s); 
    shared_ptr < Node > head = parse(s);
    cerr << genAns(head) << endl;
    gen.add(head);
    head = normalForm(head);
    // self checking
    string ss = genAns(head);
    assert(ss == genAns(normalForm(parse(genAns(head)))));
    db("OK");

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

