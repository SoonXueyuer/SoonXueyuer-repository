#include <stdio.h>

// 计算最大公约数（辗转相除法）（也叫欧几里得算法）
long long gcd(long long a, long long b) {
    while (b != 0) {
        long long temp = b;
        b = a % b;
        a = temp;
    }
    return a;
}

// 判断是否互质
int is_coprime(long long a, long long b) {
    return gcd(a, b) == 1;
}

// 自动选择一个与 phi 互质的 e
long long choose_e(long long phi) {
    for (long long e = 2; e < phi; e++) {
        if (is_coprime(e, phi)) {
            return e;
        }
    }
    return -1; // 如果没有找到合适的 e，返回 -1
}

int main() {
    long long phi;
    printf("请输入 φ(n): ");
    scanf_s("%lld", &phi);

    long long e = choose_e(phi);

    if (e != -1) {
        printf("选择出的 e = %lld (与 φ(n) 互质)\n", e);
    }
    else {
        printf("没有找到合适的 e\n");
    }

    return 0;
}
