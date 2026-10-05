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

// stack
int box[1500000];
int top = -1;

void push(int x) {
    top++;
    box[top] = x;
}

int pop() {
    int val = box[top];
    top--;
    return val;
}

bool is_empty() {
    return top == -1;
}

// 非遞迴版
int ack_iter(int m, int n) {
    top = -1; // 每次跑重設 index
    push(m);

    while (!is_empty()) {
        m = pop();

        if (m == 0) {
            // 算到底加一
            n = n + 1;
        } else if (n == 0) {
            // A(m - 1, 1)
            push(m - 1);
            n = 1;
        } else {
            // A(m - 1, A(m, n - 1))
            // 把外層的 m - 1 壓進去等，再把 m 壓進去算內層
            push(m - 1);
            push(m);
            n = n - 1;
        }
    }
    return n;
}

// 直接傳 m 和 n 跑測試印結果，不用宣告陣列傳送
void run_test(int m, int n) {
    cout << "A(" << m << ", " << n << "):" << endl;
    cout << "  rec : " << ack(m, n) << endl;
    cout << "  iter: " << ack_iter(m, n) << endl;
}

int main() {
    cout << "=== Problem 1 Ackermann Test ===" << endl;
    // 直接呼叫測試，完全不開陣列
    run_test(0, 0);
    run_test(1, 2);
    run_test(2, 2);
    run_test(3, 2);
    run_test(3, 3);

    return 0;
}
