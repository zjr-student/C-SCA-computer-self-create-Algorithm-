/**
 * ============================================================
 * MAI 编码器 v2.0
 * 
 * 核心设计：
 *   1. 所有编码统一为 2 位数字（00–99）
 *   2. 解码时每次读取 2 位，直接查表
 *   3. 变量由 66–70 自由组合而成
 * 
 * 规则简单，解码器极简，无特殊情况。
 * ============================================================
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX_TOKEN_LEN  64
#define MAX_OUTPUT_LEN 4096

/* ========================= 编码表 ========================= */

typedef struct {
    char *key;      // 人类可读的符号或关键字
    char *code;     // 对应的 2 位 MAI 编码
} MAI_Entry;

/**
 * 编码表
 * 说明：所有编码统一为 2 位数字
 */
MAI_Entry mai_table[] = {
    // ---------- 数字常量 ----------
    {"0", "00"}, {"1", "01"}, {"2", "02"}, {"3", "03"},
    {"4", "04"}, {"5", "05"}, {"6", "06"}, {"7", "07"},
    {"8", "08"}, {"9", "09"},

    // ---------- 基本运算符 ----------
    {"+", "10"}, {"-", "11"}, {"*", "12"},
    {"/", "13"}, {"%", "14"}, {"=", "15"},

    // ---------- 数学函数 ----------
    {"sqrt", "16"}, {"sin", "17"}, {"cos", "18"},
    {"log", "19"}, {"exp", "20"}, {"abs", "21"},
    {"floor", "22"}, {"ceil", "23"}, {"tan", "24"},
    {"asin", "25"}, {"acos", "26"}, {"atan", "27"},
    {"sinh", "28"}, {"cosh", "29"}, {"tanh", "30"},
    {"gamma", "31"}, {"erf", "32"},
    {"factorial", "33"}, {"gcd", "34"}, {"lcm", "35"},
    {"rand", "36"}, {"randint", "37"},

    // ---------- 数学常数 ----------
    {"pi", "38"}, {"e", "39"},

    // ---------- 幂运算 ----------
    {"^", "40"},

    // ---------- 逻辑与比较 ----------
    {"!=", "41"}, {"==", "42"}, {"<", "43"},
    {">", "44"}, {"<=", "45"}, {">=", "46"},
    {"and", "47"}, {"or", "48"}, {"not", "49"},

    // ---------- 控制流 ----------
    {"if", "50"}, {"else", "51"}, {"for", "52"},
    {"while", "53"}, {"break", "54"}, {"continue", "55"},
    {"def", "56"}, {"return", "57"}, {"in", "58"},

    // ---------- 括号与分隔符 ----------
    {"(", "59"}, {")", "60"},
    {",", "61"}, {";", "62"},

    // ---------- 常量 ----------
    {"True", "63"}, {"False", "64"}, {"None", "65"},

    // ---------- 变量代号 ----------
    {"A", "66"}, {"B", "67"}, {"C", "68"},
    {"D", "69"}, {"E", "70"},
};

/* 编码表大小 */
int mai_table_len = sizeof(mai_table) / sizeof(mai_table[0]);


/* ========================= 查找函数 ========================= */

/**
 * 根据 key（人类可读符号）查找对应的 MAI 编码
 * 
 * 参数：
 *   key - 要查找的关键字或符号（如 "sin", "+", "True"）
 * 
 * 返回值：
 *   对应的 2 位编码字符串，若未找到则返回 NULL
 */
char* encode_token(char *key) {
    for (int i = 0; i < mai_table_len; i++) {
        if (strcmp(mai_table[i].key, key) == 0) {
            return mai_table[i].code;
        }
    }
    return NULL;
}

/**
 * 根据 MAI 编码查找对应的人类可读符号
 * 
 * 参数：
 *   code - 2 位编码字符串（如 "17", "50"）
 * 
 * 返回值：
 *   对应的人类可读符号，若未找到则返回 NULL
 */
char* decode_token(char *code) {
    for (int i = 0; i < mai_table_len; i++) {
        if (strcmp(mai_table[i].code, code) == 0) {
            return mai_table[i].key;
        }
    }
    return NULL;
}


/* ========================= 编码器 ========================= */

/**
 * 将一段人类可读的 MAI 代码转换为数字编码流
 * 
 * 示例：
 *   输入: "A = 5"
 *   输出: "66 15 05"
 * 
 * 参数：
 *   input  - 人类可读的代码字符串
 *   output - 用于存放编码结果的缓冲区
 * 
 * 返回值：
 *   编码成功返回 1，失败返回 0
 */
int mai_encode(char *input, char *output) {
    if (!input || !output) return 0;
    output[0] = '\0';

    char *token = strtok(input, " \t\n");
    while (token != NULL) {
        char *code = encode_token(token);
        if (code == NULL) {
            fprintf(stderr, "错误：未识别的 token — %s\n", token);
            return 0;
        }
        strcat(output, code);
        strcat(output, " ");
        token = strtok(NULL, " \t\n");
    }
    return 1;
}


/* ========================= 解码器 ========================= */

/**
 * 将 MAI 数字编码流还原为人类可读的符号序列
 * 
 * 示例：
 *   输入: "66 15 05"
 *   输出: "A = 5"
 * 
 * 参数：
 *   input  - 以空格分隔的编码字符串（如 "66 15 05"）
 *   output - 用于存放解码结果的缓冲区
 * 
 * 返回值：
 *   解码成功返回 1，失败返回 0
 */
int mai_decode(char *input, char *output) {
    if (!input || !output) return 0;
    output[0] = '\0';

    char *token = strtok(input, " ");
    while (token != NULL) {
        // 检查编码长度是否为 2 位
        if (strlen(token) != 2) {
            fprintf(stderr, "错误：编码长度不是 2 位 — %s\n", token);
            return 0;
        }

        char *key = decode_token(token);
        if (key == NULL) {
            fprintf(stderr, "错误：未识别的编码 — %s\n", token);
            return 0;
        }

        strcat(output, key);
        strcat(output, " ");
        token = strtok(NULL, " ");
    }
    return 1;
}
