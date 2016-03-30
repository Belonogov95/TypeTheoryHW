#include "../LambdaParser/main.h"
#include "../LambdaParser/lambdaParser.h"



void solve() {
    string s;
    getline(cin, s);
    cout << genAns(parse(s)) << endl;
}

int main() {
    freopen("task1.in", "r", stdin);
    //freopen("task1.out", "w", stdout);

    solve();

    return 0;
}


