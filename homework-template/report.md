# 41143263

作業一

## 解題說明

本題要求實現一個遞迴函式，計算從 1 到 n 的連加總和。

### 解題策略

1. 使用遞迴函式將問題拆解為更小的子問題：Σ(n) = n + Σ(n - 1)
2. 當 n <= 1 時，返回 n 作為遞迴的結束條件。
3. 主程式呼叫遞迴函式，並輸出計算結果。

## 程式實作

以下為主要程式碼：

```cpp
#include <iostream>
using namespace std;

int sigma(int n) {
    if (n < 0)
        throw "n < 0";
    else if (n <= 1)
        return n;
    return n + sigma(n - 1);
}

int main() {
    int result = sigma(3);
    cout << result << '\n';
}
