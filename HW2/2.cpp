#include "../LambdaParser/main.h"
#include "../LambdaParser/lambdaParser.h"

void solve() {
    string s;
    getline(cin, s);
    auto q = genFV(parse(s));
    for (auto s: q)
        cout << s << endl;
}

int main() {
    freopen("task2.in", "r", stdin);
    //freopen("task2.out", "w", stdout); 

    solve();

    return 0;
}
