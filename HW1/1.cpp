#include "main.h"
#include "lambdaParser.h"

string s;

void read() {
    getline(cin, s);
}



string genAns(Node * v) {
    if (v->ch.empty()) {
        assert(isalpha(v->type[0]));
        return v->type;
    }
    if (v->type == "APPLY") {
        auto r1 = genAns(v->ch[0]);
        auto r2 = genAns(v->ch[1]);
        return "(" + r1 + " " + r2 + ")";
    }
    assert(v->type == "EXP");
    if (v->ch.size() == 1) return genAns(v->ch[0]);
    if (v->ch.size() == 2) return "(\\" + genAns(v->ch[0]) + "." + genAns(v->ch[1]) + ")";
    if (v->ch.size() == 3) return "(" + genAns(v->ch[0]) + "(\\" + genAns(v->ch[1]) + "." + genAns(v->ch[2]) + "))";
    assert(false);
}

void solve() {
    LambdaParser parser(s);
    Node * head = parser.parseExp();
    string ans = genAns(head);
    db(ans);
    cout << ans << endl;
}


int main() {
    freopen("task1.in", "r", stdin);
    //freopen("task1.out", "w", stdout);

    read();
    solve();


    return 0;
}


