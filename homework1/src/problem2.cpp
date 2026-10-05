#include <iostream>
#include <string>

using namespace std;

// 第2題：遞迴求所有子集合
// 直接把集合放全域，不用每一層都把陣列傳來傳去
string S[] = {"a", "b", "c"};
int n = 3;
bool first = true;

// 邏每個元素都有「挑選」與「不挑」兩條路
// cur 是目前挑出來已組好的子集合字串
void powerset(int idx, string cur) {
    // 走到最後一個位置，印出當前子集合
    if (idx == n) {
        if (!first) {
            cout << ", ";
        }
        cout << "(" << cur << ")";
        first = false;
        return;
    }

    // 分支 1：不加入 S[idx]，直接看下一個
    powerset(idx + 1, cur);

    // 分支 2：加入 S[idx]
    string next_cur = cur;
    if (next_cur.length() > 0) {
        next_cur += ",";
    }
    next_cur += S[idx];

    powerset(idx + 1, next_cur);
}

int main() {
    // 測試題目給的範例集合 S = {a, b, c}
    cout << "=== Problem 2 Powerset Test ===" << endl;
    cout << "Input set: {";
    for (int i = 0; i < n; i++) {
        cout << S[i] << (i == n - 1 ? "" : ", ");
    }
    cout << "}" << endl;

    cout << "powerset (S) = {";
    powerset(0, "");
    cout << "}" << endl;

    return 0;
}
