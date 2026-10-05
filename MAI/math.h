#ifndef MAI_MATH_H
#define MAI_MATH_H

#include <math.h>
#include <stdlib.h>
#include <string.h>

// 基础常量
#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif
#ifndef M_E
#define M_E  2.71828182845904523536
#endif

// ============================================================
// 1. 基础运算 (直接映射到 C 运算符，由编译器处理)
//    +  -  *  /  %  ^
//    这些不需要函数，直接在执行器中处理
// ============================================================

// ============================================================
// 2. 数学函数 (编码 16-37)
//    所有函数输入输出均为 double
// ============================================================

// 编码 16: sqrt
static inline double mai_sqrt(double x) {
    return sqrt(x);
}

// 编码 17: sin
static inline double mai_sin(double x) {
    return sin(x);
}

// 编码 18: cos
static inline double mai_cos(double x) {
    return cos(x);
}

// 编码 19: log (自然对数)
static inline double mai_log(double x) {
    return log(x);
}

// 编码 20: exp
static inline double mai_exp(double x) {
    return exp(x);
}

// 编码 21: abs
static inline double mai_abs(double x) {
    return fabs(x);
}

// 编码 22: floor
static inline double mai_floor(double x) {
    return floor(x);
}

// 编码 23: ceil
static inline double mai_ceil(double x) {
    return ceil(x);
}

// 编码 24: tan
static inline double mai_tan(double x) {
    return tan(x);
}

// 编码 25: asin
static inline double mai_asin(double x) {
    return asin(x);
}

// 编码 26: acos
static inline double mai_acos(double x) {
    return acos(x);
}

// 编码 27: atan
static inline double mai_atan(double x) {
    return atan(x);
}

// 编码 28: sinh
static inline double mai_sinh(double x) {
    return sinh(x);
}

// 编码 29: cosh
static inline double mai_cosh(double x) {
    return cosh(x);
}

// 编码 30: tanh
static inline double mai_tanh(double x) {
    return tanh(x);
}

// 编码 31: gamma (伽马函数)
static inline double mai_gamma(double x) {
    return tgamma(x);
}

// 编码 32: erf (误差函数)
static inline double mai_erf(double x) {
    return erf(x);
}

// 编码 33: factorial (阶乘)
static inline double mai_factorial(int n) {
    if (n < 0) return 0;
    double result = 1.0;
    for (int i = 2; i <= n; i++) {
        result *= i;
    }
    return result;
}

// 编码 34: gcd (最大公约数)
static inline int mai_gcd(int a, int b) {
    while (b != 0) {
        int t = b;
        b = a % b;
        a = t;
    }
    return a < 0 ? -a : a;
}

// 编码 35: lcm (最小公倍数)
static inline int mai_lcm(int a, int b) {
    if (a == 0 || b == 0) return 0;
    int g = mai_gcd(a, b);
    return (a / g) * b;
}

// 编码 36: rand (随机小数)
static inline double mai_rand(double min, double max) {
    return min + (max - min) * ((double)rand() / RAND_MAX);
}

// 编码 37: randint (随机整数)
static inline int mai_randint(int min, int max) {
    return min + rand() % (max - min + 1);
}

// ============================================================
// 3. 数学常量 (编码 38-39)
// ============================================================

// 编码 38: pi
static inline double mai_pi(void) {
    return M_PI;
}

// 编码 39: e
static inline double mai_e(void) {
    return M_E;
}

// ============================================================
// 4. 编码分发器 (根据 MAI 编码调用对应函数)
//    用于执行器中：code 是 2 位编码字符串，如 "17"
// ============================================================

/**
 * 根据 MAI 编码调用对应的数学函数
 * 
 * 参数：
 *   code: 2 位 MAI 编码 (如 "17" 表示 sin)
 *   args: 参数数组 (double)
 *   arg_count: 参数个数
 * 
 * 返回值：
 *   函数计算结果 (double)
 *   如果编码未知，返回 0.0
 */
static inline double mai_math_dispatch(const char *code, double *args, int arg_count) {
    if (code == NULL || args == NULL) return 0.0;
    
    // 提取编码数字
    int c = (code[0] - '0') * 10 + (code[1] - '0');
    
    switch (c) {
        // ---- 单参数函数 (arg_count >= 1) ----
        case 16: return mai_sqrt(args[0]);
        case 17: return mai_sin(args[0]);
        case 18: return mai_cos(args[0]);
        case 19: return mai_log(args[0]);
        case 20: return mai_exp(args[0]);
        case 21: return mai_abs(args[0]);
        case 22: return mai_floor(args[0]);
        case 23: return mai_ceil(args[0]);
        case 24: return mai_tan(args[0]);
        case 25: return mai_asin(args[0]);
        case 26: return mai_acos(args[0]);
        case 27: return mai_atan(args[0]);
        case 28: return mai_sinh(args[0]);
        case 29: return mai_cosh(args[0]);
        case 30: return mai_tanh(args[0]);
        case 31: return mai_gamma(args[0]);
        case 32: return mai_erf(args[0]);
        case 38: return mai_pi();
        case 39: return mai_e();
        
        // ---- 双参数函数 (arg_count >= 2) ----
        case 34: return (double)mai_gcd((int)args[0], (int)args[1]);
        case 35: return (double)mai_lcm((int)args[0], (int)args[1]);
        case 36: return mai_rand(args[0], args[1]);
        case 37: return (double)mai_randint((int)args[0], (int)args[1]);
        
        // ---- 变参函数 (阶乘) ----
        case 33: return mai_factorial((int)args[0]);
        
        default:
            return 0.0;  // 未知编码
    }
}

#endif // MAI_MATH_H
