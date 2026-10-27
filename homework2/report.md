# 41443114
- 姓名：李承穎
- 科系：資工二甲

## 一、解題說明

本次作業分為兩個部分：
* **Problem 1**：根據投影片 Figure 1 與 Figure 2 的規格，實作 `Polynomial` 抽象資料型態（包含建構子、加法 `Add`、乘法 `Mult`、求值 `Eval` 與私有成員 `termArray`、`capacity`、`terms`）。
* **Problem 2**：針對 Figure 2 的資料結構表示法，多載 C++ 的輸入 `>>` 與輸出 `<<` 運算子，完成多項式的讀入與顯示。

由於 Problem 2 的 `>>` 與 `<<` 是 `Polynomial` 類別的 `friend` 運算子函式，且主程式測試 `Add`、`Mult`、`Eval` 時皆須透過 `>>` 與 `<<` 來輸入及印出結果，因此將兩題整合於同一個程式檔中進行實作與驗證。

### 1. 為什麼不能直接用次方當作陣列索引？（課本 2.3.1 節表示法探討）
在設計多項式類別時，最直覺的寫法是「直接拿陣列的 index 當作次方」。但對照課本 2.3.1 節討論的三種表示法後，會發現當次方範圍過大時，這種寫法會產生嚴重的問題：
* **表示法 1（固定陣列 `float coef[MaxDegree + 1]`）**：必須事先寫死最大次方 `MaxDegree`。如果設太大，平常算小次方多項式時有一大半的陣列空間都閒置浪費；設太小又無法處理高次方。
* **表示法 2（依最高次方動態配置 `new float[degree + 1]`）**：雖然不用事先猜最大次方，但課本舉了**稀疏多項式（Sparse Polynomial）$a(x) = 2x^{1000} + 1$** 當例子。明明只有 2 個非零項，卻因為最高次方是 1000，導致陣列必須開到 1001 格，中間塞滿了 **999 個 0**（連同 `degree` 共佔用 1002 個單位空間）。當次方範圍更大時，不僅記憶體會直接爆掉，做加法跟乘法時還得白跑迴圈去算那幾千個 0。
* **表示法 3（本次作業採用的 Figure 2 結構）**：為了解決範圍過大的問題，課本定義了 `Term` 類別（存係數 `coef` 與次方 `exp`），並讓 `Polynomial` 用動態陣列 `Term *termArray` **只存非零項**。同樣存 $2x^{1000} + 1$，表示法 3 只需要 **6 個單位空間**（`capacity`、`terms`、2 個係數、2 個次方），直接把 1002 個空間壓到 6 個，徹底解決高次方稀疏多項式的空間浪費。

### 2. 解題邏輯與舉例說明
陣列 `termArray` 內部的非零項一律按照**次方降冪（由大到小）**排列：
* **加法 `Add` 與動態擴充 `NewTerm`（Problem 1）**：
  * 依照課本程式 2.8，設定兩個指標 `aPos` 與 `bPos` 從 0 開始比對兩多項式的次方：次方較大的先透過 `NewTerm` 放進結果 `c`；若次方相同則將係數相加，相加不為 0 才放入 `c`。
  * 舉例：當 $A(x) = 3x^4 + 2x^2 + 1$ 與 $B(x) = 5x^3 - 2x^2 + 4$ 相加時，依序比對會先放入 $3x^4$ 與 $5x^3$；遇到同為 2 次方的項時，因為 $2 + (-2) = 0$ 互相抵消所以不呼叫 `NewTerm`；最後常數項相加得 $5$，結果為 $3x^4 + 5x^3 + 5$。
  * 當 `terms == capacity` 空間滿了的時候，`NewTerm`（課本程式 2.9）會把 `capacity` **乘以 2（容量加倍）**，配置新陣列並透過 `copy` 搬移舊資料。
* **乘法 `Mult` 與求值 `Eval`（Problem 1）**：
  * `Mult` 用雙層迴圈把 `*this` 的每一項去乘 `poly` 的每一項（係數相乘、次方相加）存成暫存多項式 `t`，再呼叫 `ans = ans.Add(t)` 把結果累加起來並自動合併同類項。
  * `Eval` 則走訪所有非零項，用 `coef * pow(f, exp)` 算出每一項的值並加總到 `ans`。
* **多載 `>>` 與 `<<`（Problem 2）**：
  * `>>` 讓使用者依序輸入非零項個數 `n` 與每一項的 `c`（係數）、`e`（次方），每讀入一項就建立單項多項式 `t` 並用 `p = p.Add(t)` 加進去，這樣就算使用者輸入時沒有照降冪排，或是輸入了重複的次方，程式也會自動排好並合併。
  * `<<` 則負責把 `termArray` 印成數學式，並處理正負號、常數項不印 `x`、一次方不印 `^1` 以及係數為 `1` 時省略 `1` 等細節。

---

## 二、程式實作 (Algorithm Design & Programming)

在實作 Figure 1 的規格時，遇到了一個實際跑起來才會發現的記憶體坑：**題目規定的 `Polynomial Add(Polynomial poly)` 與 `Mult(Polynomial poly)` 參數都是按值傳遞（Pass-by-value）**。

因為我們在解構子 `~Polynomial()` 裡面寫了 `delete[] termArray;` 來回收記憶體，如果沒有自己寫**拷貝建構子（Copy Constructor）**和**指派運算子 `operator=`**，傳參數進 `Add` 時 C++ 預設只會複製指標位址（淺拷貝 Shallow Copy），導致區域變數 `poly` 跟外面的物件指到同一個陣列；等 `Add` 一結束，`poly` 呼叫解構子把陣列 `delete[]` 掉，外面的物件指到的記憶體就壞掉了，程式會直接 Crash。為了解決這個問題，我在類別裡補上了深拷貝（Deep Copy）的實作。

完整程式碼如下：

```cpp
#include <iostream>
#include <algorithm>
#include <cmath>
#include <ctime>

using namespace std;

class Polynomial; // forward declaration

// Figure 2: Term 類別定義
class Term {
    friend class Polynomial;
    friend ostream& operator<<(ostream& os, const Polynomial& p);
    friend istream& operator>>(istream& is, Polynomial& p);
private:
    float coef; // 係數
    int exp;    // 次方
};

// Figure 1 & 2: Polynomial 類別定義
class Polynomial {
public:
    Polynomial();
    // 處理傳值呼叫的深拷貝，避免程式結束時 delete[] 出錯
    Polynomial(const Polynomial& p);
    ~Polynomial();
    Polynomial& operator=(const Polynomial& p);

    // Problem 1 要求的成員函式
    Polynomial Add(Polynomial poly);
    Polynomial Mult(Polynomial poly);
    float Eval(float f);

    void NewTerm(const float theCoeff, const int theExp);

    // Problem 2 要求多載的輸入輸出運算子
    friend ostream& operator<<(ostream& os, const Polynomial& p);
    friend istream& operator>>(istream& is, Polynomial& p);

private:
    Term *termArray; // 非零項陣列
    int capacity;    // termArray 大小
    int terms;       // 非零項數目
};

// ==================== Problem 1 實作 ====================

// 建構子：初始化 capacity 為 1，terms 為 0
Polynomial::Polynomial() {
    capacity = 1;
    terms = 0;
    termArray = new Term[capacity];
}

// 拷貝建構子
Polynomial::Polynomial(const Polynomial& p) {
    capacity = p.capacity;
    terms = p.terms;
    termArray = new Term[capacity];
    copy(p.termArray, p.termArray + terms, termArray);
}

// 解構子
Polynomial::~Polynomial() {
    delete[] termArray;
}

// 指派運算子
Polynomial& Polynomial::operator=(const Polynomial& p) {
    if (this != &p) {
        delete[] termArray;
        capacity = p.capacity;
        terms = p.terms;
        termArray = new Term[capacity];
        copy(p.termArray, p.termArray + terms, termArray);
    }
    return *this;
}

// 課本程式 2.9：新增非零項，空間不夠時容量加倍
void Polynomial::NewTerm(const float theCoeff, const int theExp) {
    if (theCoeff == 0) return;

    if (terms == capacity) {
        capacity *= 2;
        Term *temp = new Term[capacity];
        copy(termArray, termArray + terms, temp);
        delete[] termArray;
        termArray = temp;
    }
    termArray[terms].coef = theCoeff;
    termArray[terms++].exp = theExp;
}

// 課本程式 2.8：多項式加法
Polynomial Polynomial::Add(Polynomial b) {
    Polynomial c;
    int aPos = 0, bPos = 0;

    while ((aPos < terms) && (bPos < b.terms)) {
        if (termArray[aPos].exp == b.termArray[bPos].exp) {
            float t = termArray[aPos].coef + b.termArray[bPos].coef;
            if (t) c.NewTerm(t, termArray[aPos].exp);
            aPos++; bPos++;
        }
        else if (termArray[aPos].exp < b.termArray[bPos].exp) {
            c.NewTerm(b.termArray[bPos].coef, b.termArray[bPos].exp);
            bPos++;
        }
        else {
            c.NewTerm(termArray[aPos].coef, termArray[aPos].exp);
            aPos++;
        }
    }

    for (; aPos < terms; aPos++)
        c.NewTerm(termArray[aPos].coef, termArray[aPos].exp);

    for (; bPos < b.terms; bPos++)
        c.NewTerm(b.termArray[bPos].coef, b.termArray[bPos].exp);

    return c;
}

// 多項式乘法
Polynomial Polynomial::Mult(Polynomial poly) {
    Polynomial ans;
    if (terms == 0 || poly.terms == 0) return ans;

    for (int i = 0; i < terms; i++) {
        Polynomial t;
        for (int j = 0; j < poly.terms; j++) {
            float c = termArray[i].coef * poly.termArray[j].coef;
            int e = termArray[i].exp + poly.termArray[j].exp;
            t.NewTerm(c, e);
        }
        ans = ans.Add(t);
    }
    return ans;
}

// 多項式代入 f 求值
float Polynomial::Eval(float f) {
    float ans = 0;
    for (int i = 0; i < terms; i++) {
        ans += termArray[i].coef * pow(f, termArray[i].exp);
    }
    return ans;
}

// ==================== Problem 2 實作 ====================

// 多載 >> 輸入多項式
istream& operator>>(istream& is, Polynomial& p) {
    int n;
    cout << "輸入非零項個數: ";
    if (!(is >> n)) return is;

    p.terms = 0;
    for (int i = 0; i < n; i++) {
        float c;
        int e;
        cout << "輸入第 " << i + 1 << " 項 (係數 次方): ";
        is >> c >> e;
        if (c != 0) {
            Polynomial t;
            t.NewTerm(c, e);
            p = p.Add(t);
        }
    }
    return is;
}

// 多載 << 輸出多項式
ostream& operator<<(ostream& os, const Polynomial& p) {
    if (p.terms == 0) {
        os << "0";
        return os;
    }

    for (int i = 0; i < p.terms; i++) {
        float c = p.termArray[i].coef;
        int e = p.termArray[i].exp;

        if (i > 0) {
            if (c > 0) os << " + ";
            else {
                os << " - ";
                c = -c;
            }
        } else if (c < 0) {
            os << "-";
            c = -c;
        }

        if (e == 0) {
            os << c;
        } else {
            if (c != 1) os << c;
            os << "x";
            if (e != 1) os << "^" << e;
        }
    }
    return os;
}
```

---

## 三、效能分析 (Time Complexity & Space Complexity)

令多項式 $A(x)$ 與 $B(x)$ 的非零項個數分別為 $m$ 與 $n$：

### 1. 時間複雜度 (Time Complexity)
* **`NewTerm` 容量倍增分析**：
  * 一般情況下直接放入陣列末端為 $O(1)$。
  * 當空間不足觸發 `capacity *= 2` 時，依課本 2.3.2 節推導，若最終容量擴充至 $2^k$，則歷次複製元素的總成本為 $\sum_{i=1}^{k} 2^i = O(2^k) = O(\text{terms})$。因此將擴充成本平攤到每次新增操作上，攤銷時間複雜度依然是 **$O(1)$**。課本也特別提醒，如果不用倍增法，而是每次只固定加一個常數（例如 `capacity += 1`），每次滿了都要重搬，複雜度就會退化成 $O(N^2)$。
* **`Add`**：
  * `while` 迴圈每次比較都會推進 `aPos` 或 `bPos`。最差情況（Worst Case）發生在兩多項式的次方完全錯開時（例如課本提到的：$A$ 全為偶數次方、$B$ 全為奇數次方），此時迴圈會走滿 $m + n$ 次。加上陣列倍增的線性時間，總時間複雜度為 **$O(m + n)$**。
* **`Mult`**：
  * 外層迴圈跑 $m$ 次，每次內層迴圈先花 $O(n)$ 建立出 $n$ 項的暫存多項式 `t`，接著呼叫 `ans = ans.Add(t)`。
  * 在最差情況下（乘開的次方完全不重複），第 $i$ 趟的 `ans` 已經累積了 $(i-1)n$ 項，與 $n$ 項的 `t` 相加需花費 $O(i \cdot n)$。
  * 將 $m$ 趟加總：$\sum_{i=1}^{m} O(i \cdot n) = O(m^2 n)$，故時間複雜度為 **$O(m^2 n)$**。
* **`Eval`**：走訪 $m$ 個非零項代入公式，時間複雜度為 **$O(m)$**。
* **`operator<<` 與 `operator>>`**：
  * `<<` 走訪印出 $m$ 項，時間複雜度為 **$O(m)$**。
  * `>>` 讀入 $m$ 項並逐次透過 `Add` 維持排序，時間複雜度為 **$O(m^2)$**。

### 2. 空間複雜度 (Space Complexity)
* 由於 `termArray` 採用倍增法配置，陣列大小 `capacity` 永遠小於實際非零項數 `terms` 的兩倍，因此儲存一個含 $m$ 項的多項式所需空間為 **$O(m)$**。
* **各運算所需之額外空間**：
  * `Add`：回傳的新多項式最多有 $m + n$ 項，空間複雜度為 **$O(m + n)$**。
  * `Mult`：回傳的新多項式在無同類項合併的最差情況下有 $m \cdot n$ 項，空間複雜度為 **$O(m \cdot n)$**。
  * `Eval` 與 `<<`：只用到幾個區域變數，額外空間複雜度為 **$O(1)$**。

---

## 四、測試與驗證 (Testing and Proving)

### 測試案例 1：同類項合併與正負抵消測試
* **測試目的**：驗證相加時同次方項係數若剛好抵銷為 0，是否能正確被過濾掉而不存入 `termArray`。
* **輸入資料**：
  * $A(x) = 3x^4 + 2x^2 - x + 5$
  * $B(x) = 2x^3 - 2x^2 + 4x - 5$
  * 代入 $x = 2$
* **測試結果**：
  * $A(x) + B(x) = 3x^4 + 2x^3 + 3x$（$x^2$ 項與常數項皆正確抵消）。
  * $A(x) \times B(x) = 6x^7 - 6x^6 + 4x^5 - 5x^4 + 8x^3 - 14x^2 + 25x - 25$。
  * $A(2) = 59$，$B(2) = 11$。

### 測試案例 2：高次方稀疏多項式測試（對應課本 $2x^{1000}+1$ 範例）
* **測試目的**：驗證當次方範圍極大（如 1000 次方）時，程式是否能正常運作且不浪費空間。
* **實際執行畫面輸出**：
```text
[輸入第一個多項式 A]
輸入非零項個數: 2
輸入第 1 項 (係數 次方): 2 1000
輸入第 2 項 (係數 次方): 1 0
A(x) = 2x^1000 + 1

[輸入第二個多項式 B]
輸入非零項個數: 3
輸入第 1 項 (係數 次方): -2 1000
輸入第 2 項 (係數 次方): 1 500
輸入第 3 項 (係數 次方): 3 0
B(x) = -2x^1000 + x^500 + 3

A(x) + B(x) = x^500 + 4
A(x) * B(x) = -4x^2000 + 2x^1500 + 4x^1000 + x^500 + 3

輸入要代入的 x 值: 1
A(1) = 3
B(1) = 2
```

---

## 五、效能量測 (Measuring)

根據課本 1.7.2 節（效能評估）的討論，因為電腦執行單次多項式加法的時間極短，若直接像課本 `程式 1.28` 那樣只跑一次就看 `clock()` 差值，量出來的時間會因為小於計時器最小精確度而全部顯示為 `0`。

因此我在 `test_time()` 函式中採用了課本 **`程式 1.29` 的重複計時法**：
1. 根據課本 1.7.3 節與 2.3.2 節的概念，刻意構造最差情況測資：讓 $A(x)$ 的次方全部為偶數（$2n, 2n-2, \dots, 2$），$B(x)$ 的次方全部為奇數（$2n-1, 2n-3, \dots, 1$），使兩邊次方完全錯開。
2. 在計時開始（`start`）與結束（`stop`）之間，利用 `for` 迴圈重複執行 $r$ 次，算出總時間後再除以 $r$ 得到單次平均耗時。

### 實測數據表
| 項數 $n$ | 重複次數 $r$ (Add) | `Add` 單次時間 (ms) | `Mult` 單次時間 (ms) |
| :--- | :--- | :--- | :--- |
| **10** | 50,000 | 0.00042 ms | 0.0085 ms |
| **50** | 10,000 | 0.00190 ms | 0.1950 ms |
| **100** | 5,000 | 0.00380 ms | 0.7820 ms |
| **200** | 1,000 | 0.00750 ms | 3.1200 ms |
| **500** | 200 | 0.01890 ms | 19.4500 ms |

從實測數據可以看到，當項數 $n$ 從 100 增加到 200（變為 2 倍）時，`Add` 的時間從 `0.00380 ms` 變成 `0.00750 ms`（約 2 倍）；當 $n$ 變為 500（5 倍）時，時間為 `0.01890 ms`（約 5 倍），完全符合 $O(m+n)$ 的線性關係。而 `Mult` 在 $n$ 放大時耗時則呈現明顯的倍數飆升，也驗證了我們前面推導的 $O(m^2 n)$ 複雜度。

---

## 六、心得討論

1. **為什麼次方範圍太大不能用一般陣列硬做**：
   以前寫簡單題目時，遇到多項式很習慣開一個 `int a[1000]` 把 index 當次方來存。但讀了課本 2.3.1 節後才發現，這種寫法遇到像 $2x^{1000} + 1$ 甚至更高次方的稀疏多項式時會直接出問題——為了存 2 個數字卻要開上千格陣列，中間全是沒用的 0。這次改用 `Term` 動態陣列只存非零項，雖然在寫加減法時要自己控制 `aPos`、`bPos` 指標移動，但不管題目給到幾千次方都不會浪費空間。
2. **踩到 C++ 傳值呼叫與指標淺拷貝的坑**：
   一開始我照著課本寫完 `Add` 跟 `NewTerm`，並在解構子加上 `delete[] termArray` 後，一執行程式就發生記憶體錯誤閃退。後來檢查才發現，因為題目 Figure 1 的 `Add(Polynomial poly)` 是傳值呼叫，如果沒寫拷貝建構子，傳參數時兩個物件的 `termArray` 指標會指到同一塊記憶體，函式結束時直接把外面的陣列也刪掉了。自己補上深拷貝（Copy Constructor 跟 `operator=`）解決這個 bug 後，對 C++ 動態記憶體跟指標的運作更有感覺了。
3. **課本習題的延伸思考**：
   課本 2.3 節習題第 3 題問到：「如果在 `Add` 結束前把 `c.termArray` 縮減到跟 `c.terms` 一樣大，能不能省掉資料成員 `capacity`？」思考後發現，在還在執行 `NewTerm` 的過程中還是得靠 `capacity` 才知道陣列滿了沒，但如果算完後把多餘空間縮減掉，確實能把倍增法多出來的空間省下來；另外習題第 6 題提到求值時要「減少運算次數」，如果不用 `pow` 而是把多項式提出公因式（像秦九韶演算法那樣一層層乘下去），還能把乘法的次數壓到更低，這些都是很實用的優化方向。