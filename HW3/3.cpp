#include "main.h"
#include "lambdaParser.h"


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

string s;
int cnt;

Node * makeSubst(Node * v, string name, Node * u) {
    if (islower(v->type[0])) {
        if (v->type == name) {
            cnt++;
            return u;
        }
        return v;
    }
    if (v->type == "APPLY") {
        v->ch[0] = makeSubst(v->ch[0], name, u);
        v->ch[1] = makeSubst(v->ch[1], name, u);
        return v;
    }
    assert(v->type == "EXP");
    if (v->ch.size() == 1) {
        v->ch[0] = makeSubst(v->ch[0], name, u);
        return v;
    }
    if (v->ch.size() == 2) {
        if (v->ch[0]->type == name) return v;      
        v->ch[1] = makeSubst(v->ch[1], name, u);
        return v;
    }
    if (v->ch.size() == 3) {
        v->ch[0] = makeSubst(v->ch[0], name, u);
        if (v->ch[1]->type != name) 
            v->ch[2] = makeSubst(v->ch[2], name, u);
        return v;
    }
    assert(false);
}

void solve() {
    getline(cin, s);
    LambdaParser parser(s);
    Node * head = parser.parseCondition();
    Node * v = head->ch[0];

    string s = head->ch[1]->type;
    Node * u = head->ch[2];
    
    Node * ans = makeSubst(v, s, u);
    if (cnt > 0) 
        cout << genAns(ans) << endl;
    else {
        cout << "expression doesn't have a free variable " << s << endl;
    }

}

int main() {
    freopen("task3.in", "r", stdin);
    //freopen("task3.out", "w", stdout);

    solve();

    return 0;
}
