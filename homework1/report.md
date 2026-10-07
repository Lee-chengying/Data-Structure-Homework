# 41443114

- 姓名：李承穎
- 科系：資工二甲

---

## 解題說明

### 問題描述
本作業的兩項題目：
1. **Problem 1 (Ackermann's Function)**：實作阿克曼函式 A(m, n)。由於該函式具有非原始遞迴與爆炸性增長的特性，極易造成系統呼叫堆疊溢位（Stack Overflow）。要求分別以「遞迴版本」與「非遞迴版本」實現。
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
// 直接把集合放全域，就不用每一層都把陣列傳來傳去
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
  * Ackermann 函式屬於非原始遞迴函式，其運算步數與數值成長極快，無法簡化為一般的多項式。
  * 當 m = 1 時，A(1, n) = n + 2，時間複雜度為 O(n)。
  * 當 m = 2 時，A(2, n) = 2n + 3，時間複雜度為 O(n)。
  * 當 m = 3 時，A(3, n) = 2^(n + 3) - 3，展開運算步數呈指數成長，時間複雜度為 O(2^n)。
  * 整體時間複雜度直接正比於輸出結果的數值大小，通常表示為 O(A(m, n))。
* **空間複雜度**：
  * **遞迴版**：系統呼叫堆疊（Call Stack）的最高深度與運算路徑深度一致，空間複雜度為 O(A(m, n))。
  * **非遞迴版**：使用自行配置的靜態陣列 `box` 作為模擬堆疊，其內部同時容納元素的最大峰值同樣為 O(A(m, n))。

### Problem 2: Powerset 冪集
* **時間複雜度**：
  * 集合中的 n 個元素在每個步驟皆有「選取」與「不選取」兩種狀態，遞迴決策樹共有 2^n 個節點。
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

這次作業主要是在練遞迴的核心觀念，還有手動刻出資料結構去模擬系統底層的 Call Stack，把原本深不見底的遞迴改成迴圈迭代來跑。

在函式庫使用方面，我只引入了 C++ 最基礎的 `<iostream>` 來做終端機的格式化輸出，以及 `<string>` 來方便處理與串接子集合的字串內容。因為這學期作業有很明確的規範，不能直接使用 include `<stack>` 或 `<vector>` 這種現成的 STL 容器，不能貪圖方便直接拿官方容器來用，所以整個堆疊的操作與記憶體空間，我全都是自己開陣列硬幹出來的。

在寫第一題 Ackermann 函式的時候，遞迴版本其實只要對照著題目給的數學條件去寫 if-else 就行了，程式碼非常短。但這題有趣的地方在於數值增長速度太快了，稍微把數字往上帶一點點（例如 m = 3, n = 3），遞迴展開的呼叫層數就會多達幾千層，系統的記憶體堆疊很容易直接爆掉發生 Stack Overflow。所以轉換成非遞迴版本就變得很有挑戰性。

因為不能用標準庫現成的 stack，我在全域直接開了一個容量達 1,500,000 的靜態陣列 `box` 當作自己的手刻堆疊，搭配一個 `top` 變數來記錄最上面的位置，並自己寫了 `push_val` 與 `pop_val`。實作非遞迴最燒腦的地方，是在處理第三種巢狀分支 A(m - 1, A(m, n - 1))：因為外層的 m - 1 必須等內層算完拿到數值後才能繼續往下算，所以堆疊推入的順序絕對不能顛倒。我必須先把還不能算的外層 m - 1 壓到堆疊底層放著等待，再把內層要算的 m 壓在最上方，然後讓 n 減 1 進入下一次迴圈去跑內層。等到內層運算觸底滿足 m = 0 把數值加 1 算出來後，再從堆疊裡把剛剛壓在底下的 m - 1 彈出來接續運算。

另外在除錯的時候，我原本習慣在全域寫一個 `bool is_empty()` 函式來判斷堆疊是否清空，結果在編譯時發現新版 C++ 編譯器會因為 `using namespace std;` 而跟 `<type_traits>` 裡的 `std::is_empty` 撞名，跳出 reference ambiguous 的錯誤。為了避免跨平台編譯器產生相容性問題，我把判斷式直接簡化成最俐落的指標判斷 `while (top >= 0)`，這樣既乾淨又不會有命名衝突。最後在測試端，我也不打算另外開一堆陣列去傳測資，而是直接寫了一個 `run_test(m, n)` 輔助函式一組一組傳進去印出結果，讓整個 `main` 函式維持最精簡的狀態。

第二題 Powerset 冪集，概念上就是深度優先搜尋（DFS）走訪一棵二元決策樹。集合裡的每一個元素，在走訪時我都切成「不加入」跟「加入」兩條路徑向下遞迴。原本一開始寫的時候，我每跑一層遞迴都會把集合陣列當作參數傳進去，但後來發現這樣會讓每次函式呼叫的 Stack Frame 傳參變得很沉重、很冗贅。於是我把集合陣列 `S`、元素個數 `n` 還有控制排版逗號的 `first` 通通移到全域變數。如此一來，遞迴函式就只要單純傳遞當前的索引位置 `idx` 和已串接好的字串 `cur`，當 DFS 一路走訪到底部邊界時直接格式化輸出，省下了層層拷貝與宣告二維陣列儲存所有組合的麻煩。

做完這次作業最大的收穫在於，以前寫遞迴時都覺得系統自動幫我們記住狀態是很理所當然的事，但這次親自用陣列刻出堆疊、手動去管變數等待與彈出的先後順序後，才真正搞清楚遞迴在記憶體底層到底是如何被展開和執行的。同時也深刻體會到，在寫 C++ 時避開標準函式庫命名衝突與精簡傳參設計的小細節有多重要，整份作業寫下來收穫非常扎實。
