#include "../LambdaParser/main.h"
#include "../LambdaParser/lambdaParser.h"

bool flag;
FreeVarGenerator gen;



Node * makeBRed(Node * v) {
    if (islower(v->type[0])) return v;
    //db(v->type);
    if (v->type == "ABSTR") {
        v->r = makeBRed(v->r);
        return v;
    }
    if (v->type == "APPLY") {
        if (v->l->type == "ABSTR") {
            int cnt = 0;
            v = makeSubst(v->l->r, v->l->l->type, v->r, cnt, gen);
            flag = 1;
            return v;
        }

        //v->r = makeBRed(v->r);
        //if (flag) return v;
        //v->l = makeBRed(v->l);
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
    gen.add(head);

    for (int it = 0; ; it++) {
        //db(it);
        if (it % 100 == 0) {
            db(clock() * 1.0 / CLOCKS_PER_SEC);
            db(it);
            db(genAns(head).size());
            //db(genAns(head));
            db(gen.cur);
        }
        flag = 0;
        head = makeBRed(head);
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

