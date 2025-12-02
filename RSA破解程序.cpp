#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/*
    ═══════════════════════════════════════════════════════════════════════════════
    RSA 破解程序
    ═══════════════════════════════════════════════════════════════════════════════

    破解原理：
    1. 分解 n = p * q，找到素因子 p 和 q
    2. 计算欧拉函数 φ(n) = (p-1) * (q-1)
    3. 暴力求私钥 d，使得 (e * d) ≡ 1 (mod φ(n))
    4. 对每个密文 c 使用 m = c^d (mod n) 解密

    适用场景：小规模 RSA（n < 10^6）
    ═══════════════════════════════════════════════════════════════════════════════
*/

// ═══════════════════════════════════════════════════════════════════════════════
// 快速模幂算法：计算 base^exp (mod mod)
// ═══════════════════════════════════════════════════════════════════════════════
int mod_pow(int base, int exp, int mod)
{
    long long result = 1;  // 结果，防止溢出使用 long long
    long long b = base % mod;  // 底数取模

    while (exp > 0)
    {
        if (exp % 2 == 1)        // 如果指数为奇数
            result = (result * b) % mod;  // 累乘到结果中

        b = (b * b) % mod;       // 底数的平方
        exp = exp / 2;           // 指数右移（除以2）
    }

    return (int)result;
}

int main()
{
    // ═══════════════════════════════════════════════════════════════════════════════
    // 变量声明
    // ═══════════════════════════════════════════════════════════════════════════════
    int n, e;                    // 公钥参数：n = p*q, e
    char ciphertext[500];        // 用户输入的密文字符串
    int nums[200];               // 存储解析后的密文数字数组
    int count = 0;               // 密文数字个数计数器
    int i;

    // ═══════════════════════════════════════════════════════════════════════════════
    // 第一步：输入公钥参数 n 和 e
    // ═══════════════════════════════════════════════════════════════════════════════
    printf("══════════════════════════════════════════════════════════════════════\n");
    printf("                        RSA 破解程序\n");
    printf("══════════════════════════════════════════════════════════════════════\n");

    printf("请输入 n（如 3233）：");
    scanf_s("%d", &n);
    printf("请输入 e（如 17）：");
    scanf_s("%d", &e);

    getchar(); // 消耗掉 scanf 留下的换行符，避免 fgets 读取空行

    // ═══════════════════════════════════════════════════════════════════════════════
    // 第二步：输入密文（空格分隔的数字）
    // ═══════════════════════════════════════════════════════════════════════════════
    printf("请输入密文（例如：2170 1313 745 745 2185 1992）：\n");
    fgets(ciphertext, sizeof(ciphertext), stdin);

    // ═══════════════════════════════════════════════════════════════════════════════
    // 第三步：解析密文字符串 → 数字数组
    // 将 "2170 1313 745" 解析为 nums[0]=2170, nums[1]=1313, nums[2]=745
    // ═══════════════════════════════════════════════════════════════════════════════
    int current = 0;  // 当前正在解析的数字
    printf("\n解析密文：");

    for (i = 0; i < strlen(ciphertext); i++)
    {
        char c = ciphertext[i];

        if (c >= '0' && c <= '9')  // 是数字字符
        {
            current = current * 10 + (c - '0');  // 十进制累积
        }
        else  // 遇到空格或其他非数字字符
        {
            if (current > 0)  // 如果当前数字有效
            {
                nums[count] = current;
                printf(" %d", current);
                count++;
                current = 0;
            }
        }
    }

    // 处理最后一个数字（字符串末尾可能没有空格）
    if (current > 0)
    {
        nums[count] = current;
        printf(" %d", current);
        count++;
    }
    printf(" (共 %d 个密文块)\n", count);

    // ═══════════════════════════════════════════════════════════════════════════════
    // 第四步：分解 n = p * q（暴力分解）
    // 从 2 开始尝试除 n，直到找到因子
    // ═══════════════════════════════════════════════════════════════════════════════
    int p = 0, q = 0;
    printf("\n正在分解 n = %d...\n", n);

    for (i = 2; i < n; i++)  // i < n/2 也可以，但这样更通用
    {
        if (n % i == 0)
        {
            p = i;
            q = n / i;
            break;
        }
    }

    if (p == 0)
    {
        printf("? 无法分解 n，可能是大素数或分解失败！\n");
        return 0;
    }

    printf("? 成功分解 n： p = %d, q = %d (验证：%d * %d = %d)\n",
        p, q, p, q, p * q);

    // ═══════════════════════════════════════════════════════════════════════════════
    // 第五步：计算欧拉函数 φ(n) = (p-1) * (q-1)
    // ═══════════════════════════════════════════════════════════════════════════════
    int phi = (p - 1) * (q - 1);
    printf("? φ(n) = (p-1) * (q-1) = %d\n", phi);

    // ═══════════════════════════════════════════════════════════════════════════════
    // 第六步：暴力求私钥 d
    // 找到 d 使得 (e * d) ≡ 1 (mod φ(n))
    // ═══════════════════════════════════════════════════════════════════════════════
    int d = -1;
    printf("正在求解私钥 d (e * d ≡ 1 (mod φ(n)))...\n");

    for (i = 1; i < phi; i++)
    {
        if ((e * i) % phi == 1)  // 找到模逆元
        {
            d = i;
            break;
        }
    }

    if (d == -1)
    {
        printf("? 未找到私钥 d，解密失败！\n");
        return 0;
    }

    printf("? 成功求得私钥 d = %d\n", d);
    printf("   验证：(e * d) mod φ(n) = (%d * %d) mod %d = %d ?\n",
        e, d, phi, (e * d) % phi);

    // ═══════════════════════════════════════════════════════════════════════════════
    // 第七步：批量解密每个密文块
    // 对每个 c_i 计算 m_i = c_i^d (mod n)，然后转为字符
    // ═══════════════════════════════════════════════════════════════════════════════
    printf("\n══════════════════════════════════════════════════════════════════════\n");
    printf("解密结果：\n");
    printf("══════════════════════════════════════════════════════════════════════\n");

    for (i = 0; i < count; i++)
    {
        int m = mod_pow(nums[i], d, n);  // m = c^d (mod n)
        printf("%c", (char)m);           // 直接输出字符
    }

    printf("\n══════════════════════════════════════════════════════════════════════\n");
    printf("? 解密完毕！\n");

    return 0;
}