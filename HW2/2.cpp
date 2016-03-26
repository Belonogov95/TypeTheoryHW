#include "main.h"
#include "lambdaParser.h"

string s;

set < string > merge(set < string > a, set < string > b) {
    for (auto s: a)
        b.insert(s);
    return b;
}

set < string > genFree(Node * v) {
    if (islower(v->type[0])) {
        assert(v->ch.empty());
        set < string > q;
        q.insert(v->type);
        return q;
    }
    if (v->type == "APPLY") {
        auto r1 = genFree(v->ch[0]);
        auto r2 = genFree(v->ch[1]);
        return merge(r1, r2);
    }
    assert(v->type == "EXP");
    if (v->ch.size() == 1) return genFree(v->ch[0]);
    if (v->ch.size() == 2) {
        auto r = genFree(v->ch[1]);
        r.erase(v->ch[0]->type);
        return r;
    }
    if (v->ch.size() == 3) {
        auto r1 = genFree(v->ch[0]);
        auto r2 = genFree(v->ch[2]);
        r2.erase(v->ch[1]->type);
        return merge(r1, r2);
    }
    assert(false);
}

void solve() {
    getline(cin, s);
    LambdaParser parser(s);
    Node * head = parser.parseExp();
    auto q = genFree(head);

    for (auto s: q)
        cout << s << endl;
}


int main() {
    freopen("task2.in", "r", stdin);
    //freopen("task2.out", "w", stdout); 

    solve();

    return 0;
}
