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

// 不能用 <stack> 只能用 array
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
