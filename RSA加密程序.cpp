#include <stdio.h>
#include <string.h>

// 计算 (base ^ exp) mod m 的快速幂函数
// RSA 中加密和解密本质就是 m = e ^ d mod n 或 c = d ^ e mod n ，直接算会超过内存
long long mod_pow(long long base, long long exp, long long mod) {
    long long result = 1;
    base %= mod;           // 防止 base > mod
    while (exp > 0) {
        if (exp & 1)       // 若exp最低位是1，则乘一次
            result = (result * base) % mod;
        exp >>= 1;         // exp 右移一位（相当于除以2）
        base = (base * base) % mod; // base = base ^ 2 mod n
    }
    return result;
}

// 计算 e 在 mod φ(n) 下的模逆 d，使得 (e * d) mod φ(n) = 1
long long mod_inverse(long long e, long long phi) {
    long long d = 0, x1 = 0, x2 = 1, y1 = 1, temp_phi = phi;
    while (e > 0) {
        long long q = temp_phi / e;
        long long t = e;
        e = temp_phi % e;
        temp_phi = t;
        t = x2;
        x2 = x1 - q * x2;
        x1 = t;
        t = y1;
        y1 = y1 - q * y1;
        y1 = t;
    }
    if (temp_phi != 1)
        return -1;  // 没有模逆
    if (x1 < 0)
        x1 += phi;
    return x1;
}

int main() {
    // 1. 选择两个素数 p 和 q（教学版使用小素数，实际 RSA 会使用 1024/2048 位）
    long long p = 61;       // 第一个素数
    long long q = 53;       // 第二个素数

    // 2. 计算 n = p * q
    // n 是 RSA 的模数，公钥和私钥都会用到
    long long n = p * q;    // RSA 模数 n = 3233

    // 3. 计算 φ(n) = (p - 1)(q - 1)
    // φ(n) 用来计算私钥 d
    long long phi = (p - 1) * (q - 1);  // φ(n) = 3120

    // 4. 选择公钥 e
    // e 必须与 φ(n) 互质，一般选较小的常用值（如 3、17、65537）
    long long e = 17;       // 公钥指数，满足 gcd(e, φ(n)) = 1，也可补充 choose_e 函数
    // 5. 计算私钥 d，使得 (e * d) mod φ(n) = 1
    long long d = mod_inverse(e, phi);  // 私钥指数 d = 2753

    // 输出 RSA 密钥（教学展示）
    printf("==== RSA 密钥生成结果 ====\n");
    printf("p = %lld\n", p);
    printf("q = %lld\n", q);
    printf("n = %lld\n", n);
    printf("φ(n) = %lld\n", phi);
    printf("公钥 (e, n) = (%lld, %lld)\n", e, n);
    printf("私钥 (d, n) = (%lld, %lld)\n", d, n);

    // 6. 让用户输入明文
    char message[256];    // 存放用户输入的明文
    printf("请输入要加密的明文: ");
    fgets(message, sizeof(message), stdin);
    int len = strlen(message);
    if (message[len - 1] == '\n')
        message[len - 1] = '\0';  // 去掉换行
    len = strlen(message);        // 重新获取长度

    long long ascii_values[256];  // 存放 ASCII 数字
    long long encrypted[256];    // 存放加密结果
    char decrypted[256];         // 存放解密后的字符

    // 7. 字符转 ASCII 数字
    printf("\n==== 字符 -> ASCII 编码 ====\n");
    for (int i = 0; i < len; i++) {
        ascii_values[i] = (int)message[i];
        printf("'%c' -> %lld\n", message[i], ascii_values[i]);
    }

    // 8. RSA 加密（对每个 ASCII 数字执行 c = m^e mod n）
    printf("\n==== 加密后的数字（密文） ====\n");
    for (int i = 0; i < len; i++) {
        encrypted[i] = mod_pow(ascii_values[i], e, n);
        printf("%lld ", encrypted[i]);
    }
    printf("\n");

    // 9. RSA 解密（m = c^d mod n），得到 ASCII 数字再转回字符
    printf("\n==== 解密得到的 ASCII ====\n");
    for (int i = 0; i < len; i++) {
        long long decrypted_ascii = mod_pow(encrypted[i], d, n);
        decrypted[i] = (char)decrypted_ascii;
        printf("%lld ", decrypted_ascii);
    }
    decrypted[len] = '\0';

    printf("\n\n==== 最终解密结果 ====\n");
    printf("%s\n", decrypted);

    return 0;
}
