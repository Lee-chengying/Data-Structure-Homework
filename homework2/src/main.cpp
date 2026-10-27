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
            p = p.Add(t); // 透過 Add 自動維持降冪排列與同次方合併
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

// ==================== 效能量測與主程式 ====================

// 參考課本 1.7.2 節程式 1.29 的計時方式
void test_time() {
    cout << "\n--- 效能量測 (單位: ms) ---\n";
    cout << "n\t重複次數r\tAdd時間\t\tMult時間\n";

    int n_arr[] = {10, 50, 100, 200, 500};
    long r_arr[] = {50000, 10000, 5000, 1000, 200};

    for (int k = 0; k < 5; k++) {
        int n = n_arr[k];
        long r = r_arr[k];

        // 產生次方完全錯開的最差情況測資 (偶數次方 vs 奇數次方)
        Polynomial a, b;
        for (int i = n; i >= 1; i--) {
            a.NewTerm(1, 2 * i);
            b.NewTerm(1, 2 * i - 1);
        }

        clock_t start = clock();
        for (long i = 1; i <= r; i++) {
            Polynomial c = a.Add(b);
        }
        clock_t stop = clock();
        double time1 = (double)(stop - start) / CLOCKS_PER_SEC * 1000.0 / r;

        long r2 = (r / 100 > 0) ? (r / 100) : 1;
        start = clock();
        for (long i = 1; i <= r2; i++) {
            Polynomial d = a.Mult(b);
        }
        stop = clock();
        double time2 = (double)(stop - start) / CLOCKS_PER_SEC * 1000.0 / r2;

        cout << n << "\t" << r << "\t\t" << time1 << "\t\t" << time2 << "\n";
    }
}

int main() {
    Polynomial a, b;

    cout << "[輸入第一個多項式 A]\n";
    cin >> a;
    cout << "A(x) = " << a << "\n\n";

    cout << "[輸入第二個多項式 B]\n";
    cin >> b;
    cout << "B(x) = " << b << "\n\n";

    cout << "A(x) + B(x) = " << a.Add(b) << "\n";
    cout << "A(x) * B(x) = " << a.Mult(b) << "\n\n";

    float x;
    cout << "輸入要代入的 x 值: ";
    if (cin >> x) {
        cout << "A(" << x << ") = " << a.Eval(x) << "\n";
        cout << "B(" << x << ") = " << b.Eval(x) << "\n";
    }

    test_time();

    return 0;
}