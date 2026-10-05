# 41443114

- 姓名：李承穎
- 科系：資工二甲

---

## 解題說明

### 問題描述
這次作業要解兩題經典的題目：
1. **Problem 1 (Ackermann's Function)**：寫阿克曼函式 A(m, n)，這函式數字大，很容易把記憶體吃爆。題目要求我們同時寫出「遞迴版」跟「非遞迴版」。
2. **Problem 2 (Powerset of a Set)**：給一個有 n 個元素的集合 S，用遞迴把所有可能的子集合全部找出來印出來。

### 解題策略
* **Problem 1**：
  * **遞迴版**：直接對著題目給的數學條件寫 if-else。遇到 m=0 就回傳 n+1；n=0 就呼叫 A(m-1, 1)；最麻煩的，要先讓內層的 A(m, n-1) 算完，再把結果整包丟給外層的 A(m-1, ...)。
  * **非遞迴版**：`<stack>` 不能拿來用，所以我自己開了一個大陣列 box 當作陽春的 stack，再用一個 top 變數來記錄位置做 push 跟 pop，直接以 `while (top >= 0)` 控制堆疊循環。
  * **測試方式**：不用宣告陣列傳來傳去，寫一個 `run_test(m, n)` 函式直接傳值呼叫測試。
* **Problem 2**：
  * 用二元樹「選或不選」的想法。走過集合裡的每個字元時，都分成兩條路。
  * 把集合放在全域，遞迴函式就不用每層傳送陣列，只記錄現在走到第幾個 idx 跟字串 cur。遞迴往下走到最後一個元素時，就代表湊好一組子集合了，直接格式化印出來。

---

## 程式實作

### Problem 1 (`src/problem1.cpp`)
```cpp
#include <iostream>

using namespace std;

// 第1題：Ackermann 函式
// 照題目寫的遞迴
int ack(int m, int n) {
    if (m == 0) {
        return n + 1;
    }
    if (n == 0) {
        return ack(m - 1, 1);
    }
    // 內層算完丟給外層
    return ack(m - 1, ack(m, n - 1));
}

// 手刻 stack，不能用 <stack> 只能用 array
int box[1500000];
int top = -1;

void push_val(int x) {
    top++;
    box[top] = x;
}

int pop_val() {
    int val = box[top];
    top--;
    return val;
}

// 非遞迴版
int ack_iter(int m, int n) {
    top = -1; // 每次跑重設 index
    push_val(m);

    while (top >= 0) {
        m = pop_val();

        if (m == 0) {
            // 算到底加一
            n = n + 1;
        } else if (n == 0) {
            // A(m - 1, 1)
            push_val(m - 1);
            n = 1;
        } else {
            // A(m - 1, A(m, n - 1))
            // 把外層的 m - 1 壓進去等，再把 m 壓進去算內層
            push_val(m - 1);
            push_val(m);
            n = n - 1;
        }
    }
    return n;
}

// 輔助函式：直接傳 m, n 跑測試，不用開陣列傳來傳去
void run_test(int m, int n) {
    cout << "A(" << m << ", " << n << "):" << endl;
    cout << "  rec : " << ack(m, n) << endl;
    cout << "  iter: " << ack_iter(m, n) << endl;
}

int main() {
    cout << "=== Problem 1 Ackermann Test ===" << endl;

    // 直接一組一組測，完全不用陣列
    run_test(0, 0);
    run_test(1, 2);
    run_test(2, 2);
    run_test(3, 2);
    run_test(3, 3);

    return 0;
}
```

### Problem 2 (`src/problem2.cpp`)
```cpp
#include <iostream>
#include <string>

using namespace std;

// 第2題：遞迴求所有子集合 (Powerset)
// 直接把集合放全域，不用每一層都把陣列傳來傳去
string S[] = {"a", "b", "c"};
int n = 3;
bool first = true;

// 邏輯：每個元素都有「挑選」與「不挑」兩條路
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
```

---

## 效能分析

### Problem 1: Ackermann 函式
* **時間複雜度**：
  * Ackermann 函式的成長速度快得嚇人，沒辦法寫成一般的多項式。
  * $m = 1$ 的時候，$A(1, n) = n + 2$，時間是 $O(n)$。
  * $m = 2$ 的時候，$A(2, n) = 2n + 3$，時間也是 $O(n)$。
  * 但到了 $m = 3$，$A(3, n) = 2^{n+3} - 3$，時間直接飆到 $O(2^n)$。
  * 整體的時間複雜度會直接跟著算出來的數值等比暴增，通常表示為 $O(A(m, n))$。
* **空間複雜度**：
  * **遞迴版本**：呼叫深度主要看 Call Stack 疊了多深，空間複雜度是 $O(A(m, n))$。
  * **非遞迴版本**：我自己開了 `box` 陣列當作堆疊，裡面最多同時放的元素量也是 $O(A(m, n))$。

### Problem 2: Powerset 冪集
* **時間複雜度**：
  * 每個元素都有「挑」跟「不挑」兩種選擇，整棵遞迴樹展開底部會有 $2^n$ 個組合。
  * 每次到底部印出字串時要花大約 $O(n)$ 的時間拼裝與輸出。
  * 所以總時間複雜度大約是 $O(n \times 2^n)$。
* **空間複雜度**：
  * 遞迴最大的深度剛好就是集合元素的個數 $n$。
  * 每一層只保存當前挑選字串的狀態，空間複雜度就是 $O(n)$。

---

## 測試與驗證

### 測試環境
- 編譯器：g++ (GCC)
- 終端機環境：Shell / PowerShell

### Problem 1 執行結果
```shell
$ g++ homework1/src/problem1.cpp -o homework1/src/problem1.exe
$ ./homework1/src/problem1.exe
=== Problem 1 Ackermann Test ===
A(0, 0):
  rec : 1
  iter: 1
A(1, 2):
  rec : 4
  iter: 4
A(2, 2):
  rec : 7
  iter: 7
A(3, 2):
  rec : 29
  iter: 29
A(3, 3):
  rec : 61
  iter: 61
```

### Problem 2 執行結果
```shell
$ g++ homework1/src/problem2.cpp -o homework1/src/problem2.exe
$ ./homework1/src/problem2.exe
=== Problem 2 Powerset Test ===
Input set: {a, b, c}
powerset (S) = {(), (c), (b), (b,c), (a), (a,c), (a,b), (a,b,c)}
```

---

## 申論及開發報告

寫第一題 Ackermann 的時候，遞迴版基本上只要照著題目定義寫就很順。但非遞迴版就比較麻煩，因為這學期規定不能直接 include <stack>，不能偷懶用現成的容器。

所以我直接開了一個全域陣列 box 來當作自己的 stack 用。寫非遞迴最燒腦的地方在於 A(m - 1, A(m, n - 1))：外層的 m - 1 一定要等內層的結果出來才能算，所以不能急著算它，得先把 m - 1 丟進 stack 裡面放著，接著把 m 壓進去，然後把 n 減 1 讓迴圈先去跑內層。手刻一個 stack 來跑迴圈之後，感覺對底層遞迴在記憶體裡推疊的運作方式清楚很多。而在 main 函式測試時，直接寫一個 run_test 函式單純傳整數進去測，不用多開測資陣列傳來傳去，整體看起來更簡潔。

第二題 Powerset 就單純不少，本質上就是二元樹，我都分成「不加進去」跟「加進去」兩種遞迴分支。我把集合陣列直接放在全域，函式不用層層傳陣列指標，只記錄索引跟目前字串。靠著 DFS 的特性，走到底剛好就可以印出一種組合，不用開一堆額外的陣列來存答案，寫起來乾淨很多。
