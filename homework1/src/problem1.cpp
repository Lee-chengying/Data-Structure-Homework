#include <iostream>

using namespace std;

// 第1題：Ackermann 函式
// 照題目寫的遞迴
int ackermann_rec(int m, int n) {
    if (m == 0) {
        return n + 1;
    }
    if (n == 0) {
        return ackermann_rec(m - 1, 1);
    }
    // 內層算完丟給外層
    return ackermann_rec(m - 1, ackermann_rec(m, n - 1));
}

//  capoo_box 
int capoo_box[1500000];
int top = -1;

void push_val(int x) {
    top++;
    capoo_box[top] = x;
}

int pop_val() {
    int val = capoo_box[top];
    top--;
    return val;
}

bool is_empty() {
    return top == -1;
}

// 非遞迴版
int ackermann_iter(int m, int n) {
    top = -1; // 每次跑重設 index
    push_val(m);

    while (!is_empty()) {
        m = pop_val();

        if (m == 0) {
            // 算到底加一
            n = n + 1;
        } else if (n == 0) {
            //  A(m - 1, 1)
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

int main() {
    //測試
    int test_m[] = {0, 1, 2, 3, 3};
    int test_n[] = {0, 2, 2, 2, 3};

    cout << "=== Problem 1 Ackermann Test ===" << endl;
    for (int i = 0; i < 5; i++) {
        int m = test_m[i];
        int n = test_n[i];
        cout << "A(" << m << ", " << n << "):" << endl;
        cout << "  rec : " << ackermann_rec(m, n) << endl;
        cout << "  iter: " << ackermann_iter(m, n) << endl;
    }

    return 0;
}
