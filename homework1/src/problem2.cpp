#include <iostream>
#include <string>

using namespace std;

// 第2題：遞迴求所有子集合
// sub 挑出來已組好的子集合字串
void power_set(string set_arr[], int n, int idx, string sub, bool &first) {
    // 走到最後一個位置，印出當前子集合
    if (idx == n) {
        if (!first) {
            cout << ", ";
        }
        cout << "(" << sub << ")";
        first = false;
        return;
    }

    // 分支 1
    power_set(set_arr, n, idx + 1, sub, first);

    // 分支 2：多 set_arr[idx]
    string next_sub = sub;
    if (next_sub.length() > 0) {
        next_sub += ",";
    }
    next_sub += set_arr[idx];

    power_set(set_arr, n, idx + 1, next_sub, first);
}

int main() {
    // 測試題目給的範例集合 S = {a, b, c}
    string S[3] = {"a", "b", "c"};
    int n = 3;

    cout << "=== Problem 2 Powerset Test ===" << endl;
    cout << "Input set: {";
    for (int i = 0; i < n; i++) {
        cout << S[i] << (i == n - 1 ? "" : ", ");
    }
    cout << "}" << endl;

    cout << "powerset (S) = {";
    bool first = true;
    power_set(S, n, 0, "", first);
    cout << "}" << endl;

    return 0;
}
