#include "main.h"
#include "lambdaParser.h"



void solve() {
    string s;
    getline(cin, s);
    LambdaParser parser(s);
    Node * head = parser.parseCondition();

    Node * v = head->l;

    string name = head->r->l->type;
    Node * u = head->r->r;
    
    int cnt = 0;
    Node * ans = makeSubst(v, name, u, cnt);
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
