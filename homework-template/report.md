# 41443114

- 姓名：李承穎
- 科系：資工二甲

---

## 解題說明

### 問題描述
本作業旨在解決資料結構與演算法中的兩項經典題目：
1. **Problem 1 (Ackermann's Function)**：實作阿克曼函式 A(m, n)。由於該函式具有非原始遞迴與爆炸性增長的特性，極易造成系統呼叫堆疊溢位（Stack Overflow）。本題要求分別以「遞迴版本」與「非遞迴版本」實現。
2. **Problem 2 (Powerset of a Set)**：給定一個包含 n 個相異元素的集合 S，利用遞迴決策樹列舉並輸出其所有子集合（冪集，共 2^n 個組合）。

### 解題策略
* **Problem 1**：
  * **遞迴實作**：依據題目定義的分段條件進行設計。當 m = 0 時直接返回 n + 1；當 n = 0 時呼叫 A(m - 1, 1)；其餘情況則先遞迴求解內層 A(m, n - 1)，再將所得數值傳入外層 A(m - 1, ...)。
  * **非遞迴實作**：由於規範禁止使用標準容器庫 `<stack>`，因此自行配置一個全域靜態陣列 `box` 與索引指標 `top` 來手刻堆疊。當處理最複雜的分支時，先將外層待處理的數值 m - 1 推入堆疊暫存，再將 m 推入並將 n 減 1 優先計算內層，以迴圈搭配堆疊模擬呼叫行為。
  * **測試架構**：在 `main` 函式中撰寫 `run_test(m, n)` 測試輔助函式，直接傳入測試參數進行計算與驗證，避免額外宣告與傳送陣列。
* **Problem 2**：
  * 採用二元狀態樹概念，對於集合中的每個元素皆有「選入」與「不選入」兩條遞迴分支。
  * 將集合陣列與大小定義於全域範圍，避免在遞迴展開時反覆傳遞陣列指標。以索引值 `idx` 追蹤層數，並以字串累加組合，當走訪至最深層時將當前子集合格式化印出。

---

## 程式實作

### Problem 1 (`src/problem1.cpp`)
```cpp
#include <iostream>

using namespace std;

// 第1題：Ackermann 函式
// 照題目寫遞迴
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
  * Ackermann 函式屬於非原始遞迴函式，其運算步數與數值成長極快，無法簡化為一般的多項式表示。
  * 當 m = 1 時，A(1, n) = n + 2，時間複雜度為 O(n)。
  * 當 m = 2 時，A(2, n) = 2n + 3，時間複雜度為 O(n)。
  * 當 m = 3 時，A(3, n) = 2^(n + 3) - 3，展開運算步數呈指數成長，時間複雜度為 O(2^n)。
  * 整體時間複雜度直接正比於輸出結果的數值大小，通常表示為 O(A(m, n))。
* **空間複雜度**：
  * **遞迴版**：系統呼叫堆疊（Call Stack）的最高深度與運算路徑深度一致，空間複雜度為 O(A(m, n))。
  * **非遞迴版**：使用自行配置的靜態陣列 `box` 作為模擬堆疊，其內部同時容納元素的最大峰值同樣為 O(A(m, n))。

### Problem 2: Powerset 冪集
* **時間複雜度**：
  * 集合中的 n 個元素在每個步驟皆有「選取」與「不選取」兩種狀態，遞迴決策樹共有 2^n 個葉節點。
  * 每次遞迴抵達底部印出子集合時，字串輸出耗費 O(n) 時間。
  * 總時間複雜度為 O(n * 2^n)。
* **空間複雜度**：
  * 遞迴最大深度即為集合元素數量 n，呼叫堆疊最大空間為 O(n)。

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

本次作業主要深入探討兩大核心主題：利用遞迴解決經典數學與組合問題，以及如何藉由自建資料結構手動模擬執行階段系統堆疊（Call Stack），將深度巢狀遞迴改寫為迭代架構。在開發工具與環境規範方面，本專案僅引用 C++ 標準程式庫中的 `<iostream>` 負責終端機的格式化串流輸出入，以及 `<string>` 進行子集合字串的串接與動態組裝；同時嚴格遵循課程規範，未引用 `<stack>`、`<vector>` 等 STL 容器程式庫，完全透過自建陣列來掌握記憶體配置與堆疊操作。

### 1. Problem 1：Ackermann 函式的遞迴實作與迭代轉換分析
Ackermann 函式是計算理論中著名的「非原始遞迴函式（Non-primitive Recursive Function）」，其輸出值與運算步數會隨著參數增加產生超指數等級（Superexponential）的爆炸性增長。
* **遞迴實作的限制**：遞迴版本雖然程式碼簡短且能完全對應數學定義，但由於每個未完成的函式呼叫都會在作業系統的執行堆疊（Call Stack）中建立新的 Stack Frame（包含返回位址、參數與暫存狀態）。當輸入值稍微提高（例如 $m=3, n=3$），呼叫層數便達到數千層，極易耗盡預設的堆疊記憶體而造成程式崩潰（Stack Overflow）。
* **迭代實作與手刻堆疊機制**：為了在非遞迴版本中重現執行期堆疊的行為，我在全域配置了一個大小達 1,500,000 的靜態整數陣列 `box`，並搭配整數指標 `top` 實作 LIFO（後進先出）的基礎操作（`push_val` 與 `pop_val`）。
* **巢狀遞迴的狀態保存邏輯**：演算法中最關鍵的難點在於處理第三條分支 $A(m-1, A(m, n-1))$。外層運算 $A(m-1, \dots)$ 必須依賴內層運算 $A(m, n-1)$ 計算完畢後的數值作為其第二參數。因此在狀態推入時，必須嚴格遵守運算優先順序：先將外層等待計算的 $m - 1$ 推入堆疊底層保存，接著將代表內層運算的 $m$ 推入堆疊頂端，並將 $n$ 遞減 1（$n = n - 1$）作為下一次迭代的起始條件。當內層運算觸底滿足 $m = 0$ 時，數值計算完成（$n = n + 1$），此時再將堆疊中等待的外層 $m - 1$ 彈出，並以剛算出的 $n$ 繼續展開後續運算，成功以迴圈完整還原深層遞迴。
* **命名空間安全與測試封裝**：在編譯過程中發現，若在全域定義 `bool is_empty()`，容易與 C++ 標準函式庫 `<type_traits>` 中的 `std::is_empty` 模板型別特徵產生命名衝突。為提升程式跨編譯器的相容性與執行效率，我將迴圈控制條件直接簡化為純指標判斷 `while (top >= 0)`。同時，在 `main()` 函式端設計了獨立的 `run_test(int m, int n)` 輔助函式，直接傳值呼叫各組測試邊界條件，免除額外宣告動態或靜態陣列傳遞測資的繁瑣記憶體開銷。

### 2. Problem 2：Powerset 冪集生成的決策樹設計與空間優化
求集合的冪集（Powerset）本質上是列舉所有狀態組合，共有 $2^n$ 種可能性。
* **二元樹分支邏輯**：本題採用深度優先搜尋（DFS）遍歷二元決策樹。演算法將每個集合元素視為一個決策節點，分支一代表「不納入當前元素」，分支二代表「將當前元素加入子集合」。透過將當前組裝字串 `cur` 傳入子節點，遞迴探訪至第 $n$ 層邊界時即代表完成一個完整的子集組合，直接將其輸出。
* **全域配置與傳參簡化**：在初始架構中，若每一層遞迴都以參數形式傳遞陣列指標或常數參照，會增加 Stack Frame 的傳參負擔。因此我將原集合陣列 `S`、元素總數 `n` 與標點符號控制標記 `first` 移至全域範圍。此改動使得 `powerset(int idx, string cur)` 僅需追蹤當前決策索引與路徑字串，大幅降低函式呼叫的記憶體開銷與程式碼耦合度。

### 3. 開發總結與收穫
透過本次實作，我深入理解了高階程式語言在執行遞迴時底層硬體與作業系統的記憶體消長過程。手刻堆疊的經驗不僅使我清楚掌握如何將抽象的遞迴狀態轉化為實體的資料儲存，也體會到以靜態記憶體配置代替動態配置在執行效能與避免溢位上的優勢。此外，在實作輸出格式與跨平台編譯的過程中，體認到避免標準函式庫命名衝突與維持程式碼簡練的重要性，對於資料結構底層運作原理有了更扎實的認識。
在 Problem 2 的 Powerset 冪集生成中，本質上是利用深度優先搜尋（DFS）遍歷二元決策樹。為優化程式結構，我將測試集合陣列與長度定義於全域變數，使遞迴函式簽名大幅簡化，無需在每一層呼叫時反覆拷貝或傳遞陣列指標，僅需傳入當前索引 `idx` 與組裝字串 `cur`。當走訪至邊界時直接格式化輸出，兼顧了執行效率與記憶體整潔度。透過本次作業，更深入體會到高階語言遞迴在底層堆疊的操作本質，以及自建資料結構在資源控制上的彈性。
