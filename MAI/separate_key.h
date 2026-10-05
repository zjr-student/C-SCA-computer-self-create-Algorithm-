#ifndef SEPARATE_KEY_H
#define SEPARATE_KEY_H

#include"string.h"
#include"stdio.h"
#include <stddef.h>

const char *mai_keywords[] = {
    "abs",
    "acos",
    "and",
    "asin",
    "atan",
    "break",
    "ceil",
    "continue",
    "cos",
    "cosh",
    "def",
    "else",
    "erf",
    "exp",
    "factorial",
    "False",
    "floor",
    "for",
    "gcd",
    "gpu",
    "if",
    "in",
    "lcm",
    "log",
    "None",
    "not",
    "or",
    "rand",
    "randint",
    "return",
    "sin",
    "sinh",
    "sqrt",
    "tan",
    "tanh",
    "True",
    "while",
    NULL
};

// 追加一个字符到字符串末尾
void tree_append_char(char *dest, char c)
{
    if (dest == NULL) return;
    size_t len = strlen(dest);
    dest[len] = c;
    dest[len + 1] = '\0';
}

// 清空字符串缓冲区（逻辑清空，写结束符）
void clear_new_variable(char *buf)
{
    if(buf == NULL)
        return;
    buf[0] = '\0';
}

/**
 * @brief 词法分词，输出写入 out_buf
 * @param code 输入源码
 * @param out_buf 外部提供输出缓冲区
 * @param out_buf_sz 缓冲区字节大小
 */
void separate(char *code, char *out_buf, size_t out_buf_sz)
{
    if(out_buf == NULL || out_buf_sz < 2)
    {
        printf("ERROR: output buffer invalid\n");
        return;
    }
    out_buf[0] = '\0'; //输出缓冲区初始化为空

    int code_len = (int)strlen(code);
    int i = code_len; //逆向扫描计数器

//辅助宏：安全追加token字符串到输出缓冲区，防止溢出
#define APPEND_TOKEN(tok_str) do{ \
    char tmp[1024]; \
    snprintf(tmp, sizeof(tmp), "{\"%s\"}", tok_str); \
    if(strlen(out_buf)+strlen(tmp)+1 < out_buf_sz){ \
        strcat(out_buf, tmp); \
    }else{ \
        printf("ERROR: token buffer overflow\n"); \
        return; \
    } \
}while(0)

    while(i != 0)
    {
        // 【修复1】循环内定义ch，每轮取当前字符
        char ch = code[code_len - i];

        if(ch == 'A' || ch == 'B' || ch == 'C' || ch == 'D' || ch == 'E')
        {
            // 赋值变量开头，例如 A=xxx,
            char new_variable[256] = {0};
            while( i!=0 && code[code_len - i] != '=' )
            {
                tree_append_char(new_variable, code[code_len - i]);
                i = i - 1;
            }
            if(i <= 0)
            {
                printf("ERROR: can not find '=' for assignment\n");
                return;
            }

            //变量名校验
            int var_ok = 1;
            size_t vlen = strlen(new_variable);
            for(size_t j = 0; j < vlen; j++)
            {
                char c = new_variable[j];
                if( !(c=='A'||c=='B'||c=='C'||c=='D'||c=='E') )
                {
                    var_ok = 0;
                    break;
                }
            }
            if(!var_ok)
            {
                printf("ERROR:Incorrect variable name\n");
                return;
            }
            APPEND_TOKEN(new_variable);
            APPEND_TOKEN("=");

            clear_new_variable(new_variable);
            i = i - 1; //跳过 '='

            //读取等号后面直到逗号
            while( i!=0 && code[code_len - i] != ',' )
            {
                tree_append_char(new_variable, code[code_len - i]);
                i = i - 1;
            }
            if(i <= 0)
            {
                printf("ERROR: missing comma after assign value\n");
                return;
            }
            i = i - 1; //跳过逗号

            //校验赋值右值：数字 或 A‑E变量
            size_t rlen = strlen(new_variable);
            int rhs_ok = 1;
            for(size_t j=0;j<rlen;j++)
            {
                char c = new_variable[j];
                if( ! ( (c >= '0' && c <= '9') || (c=='A'||c=='B'||c=='C'||c=='D'||c=='E') ) )
                {
                    rhs_ok = 0;
                    break;
                }
            }
            if(!rhs_ok)
            {
                printf("ERROR:variable accept not Number or variable\n");
                return;
            }
            APPEND_TOKEN(new_variable);
            APPEND_TOKEN(",");
            continue;
        }

        if(ch == 'a')
        {
            // abs
            if( (i+2 <= code_len) && code[code_len - i +1]=='b' && code[code_len - i +2]=='s' )
            {
                APPEND_TOKEN("abs");
                i = i - 1;
                if( i<=0 || code[code_len - i] != '(' )
                {
                    printf("Translation function has an unclosed bracket.\n");
                    return;
                }
                i = i - 1; //跳过'('

                char sub_buf[256] = {0};
                char new_variable[256] = {0};
                while( i!=0 && code[code_len - i] != ')' )
                {
                    tree_append_char(new_variable, code[code_len - i]);
                    i = i -1;
                }
                if(i <= 0)
                {
                    printf("ERROR: missing ')' for abs()\n");
                    return;
                }
                separate(new_variable, sub_buf, sizeof(sub_buf));
                APPEND_TOKEN(sub_buf);

                if(code[code_len - i] != ')')
                {
                    printf("Function is unclosed\n");
                    return;
                }
                i = i -1;
                if(i<=0 || code[code_len - i] != ',')
                {
                    printf("Without a delimiter, it cannot be identified.\n");
                    return;
                }
                i = i - 1;
                continue;
            }

            //acos
            if( (i+3 <= code_len) && code[code_len - i +1]=='c' && code[code_len - i +2]=='o' && code[code_len - i +3]=='s' )
            {
                APPEND_TOKEN("acos");
                i = i -4;
                if( i<=0 || code[code_len - i] != '(' )
                {
                    printf("Translation function has an unclosed bracket.\n");
                    return;
                }
                i = i -1;
                char sub_buf[256]={0};
                char new_variable[256]={0};
                while(i!=0 && code[code_len - i] != ')')
                {
                    tree_append_char(new_variable, code[code_len - i]);
                    i = i -1;
                }
                separate(new_variable, sub_buf, sizeof(sub_buf));
                APPEND_TOKEN(sub_buf);

                if(code[code_len - i] != ')')
                {
                    printf("Function is unclosed\n");
                    return;
                }
                i = i -1;
                if(i<=0 || code[code_len - i] != ',')
                {
                    printf("Without a delimiter, it cannot be identified.\n");
                    return;
                }
                i = i -1;
                continue;
            }

            //asin
            if( (i+3 <= code_len) && code[code_len - i +1]=='s' && code[code_len - i +2]=='i' && code[code_len - i +3]=='n' )
            {
                APPEND_TOKEN("asin");
                i = i -4;
                if( i<=0 || code[code_len - i] != '(' )
                {
                    printf("Translation function has an unclosed bracket.\n");
                    return;
                }
                i = i -1;
                char sub_buf[256]={0};
                char new_variable[256]={0};
                while(i!=0 && code[code_len - i] != ')')
                {
                    tree_append_char(new_variable, code[code_len - i]);
                    i = i -1;
                }
                separate(new_variable, sub_buf, sizeof(sub_buf));
                APPEND_TOKEN(sub_buf);

                if(code[code_len - i] != ')')
                {
                    printf("Function is unclosed\n");
                    return;
                }
                i = i -1;
                if(i<=0 || code[code_len - i] != ',')
                {
                    printf("Without a delimiter, it cannot be identified.\n");
                    return;
                }
                i = i -1;
                continue;
            }

            //atan
            if( (i+3 <= code_len) && code[code_len - i +1]=='t' && code[code_len - i +2]=='a' && code[code_len - i +3]=='n' )
            {
                APPEND_TOKEN("atan");
                i = i -4;
                if( i<=0 || code[code_len - i] != '(' )
                {
                    printf("Translation function has an unclosed bracket.\n");
                    return;
                }
                i = i -1;
                char sub_buf[256]={0};
                char new_variable[256]={0};
                while(i!=0 && code[code_len - i] != ')')
                {
                    tree_append_char(new_variable, code[code_len - i]);
                    i = i -1;
                }
                separate(new_variable, sub_buf, sizeof(sub_buf));
                APPEND_TOKEN(sub_buf);

                if(code[code_len - i] != ')')
                {
                    printf("Function is unclosed\n");
                    return;
                }
                i = i -1;
                if(i<=0 || code[code_len - i] != ',')
                {
                    printf("Without a delimiter, it cannot be identified.\n");
                    return;
                }
                i = i -1;
                continue;
            }

            // and
            if( (i+2 <= code_len) && code[code_len - i +1]=='n' && code[code_len - i +2]=='d' )
            {
                APPEND_TOKEN("and");
                i = i -3;
                continue;
            }
        }//end if(ch == 'a')

        if(ch == 'b')
        {
            APPEND_TOKEN("break");
            i = i -7;
            continue;
        }

        if(ch == 'c')
        {
            // cos / cosh
            if( (i+2 <= code_len) && code[code_len - i +1]=='o' && code[code_len - i +2]=='s' )
            {
                if( (i+3 <= code_len) && code[code_len - i +3] == 'h' )
                {
                    // cosh
                    APPEND_TOKEN("cosh");
                    i = i -4;
                    if(i<=0 || code[code_len - i]!='(')
                    {
                        printf("Translation function has an unclosed bracket.\n");
                        return;
                    }
                    i = i -1;
                    char sub_buf[256]={0};
                    char new_variable[256]={0};
                    while(i!=0 && code[code_len - i]!=')')
                    {
                        tree_append_char(new_variable, code[code_len - i]);
                        i = i-1;
                    }
                    separate(new_variable, sub_buf, sizeof(sub_buf));
                    APPEND_TOKEN(sub_buf);

                    if(code[code_len - i]!=')')
                    {
                        printf("Function is unclosed\n");
                        return;
                    }
                    i = i-1;
                    if(i<=0 || code[code_len - i]!=',')
                    {
                        printf("Without a delimiter, it cannot be identified.\n");
                        return;
                    }
                    i = i-1;
                }
                else
                {
                    //cos
                    APPEND_TOKEN("cos");
                    i = i -3;
                    if(i<=0 || code[code_len - i]!='(')
                    {
                        printf("Translation function has an unclosed bracket.\n");
                        return;
                    }
                    i = i -1;
                    char sub_buf[256]={0};
                    char new_variable[256]={0};
                    while(i!=0 && code[code_len - i]!=')')
                    {
                        tree_append_char(new_variable, code[code_len - i]);
                        i = i-1;
                    }
                    separate(new_variable, sub_buf, sizeof(sub_buf));
                    APPEND_TOKEN(sub_buf);

                    if(code[code_len - i]!=')')
                    {
                        printf("Function is unclosed\n");
                        return;
                    }
                    i = i-1;
                    if(i<=0 || code[code_len - i]!=',')
                    {
                        printf("Without a delimiter, it cannot be identified.\n");
                        return;
                    }
                    i = i-1;
                }
                continue;
            }

            // ceil
            if( (i+3 <= code_len) && code[code_len - i +1]=='e' && code[code_len - i +2]=='i' && code[code_len - i +3]=='l' )
            {
                APPEND_TOKEN("ceil");
                i = i -4;
                if(i<=0 || code[code_len - i]!='(')
                {
                    printf("Translation function has an unclosed bracket.\n");
                    return;
                }
                i = i -1;
                char sub_buf[256]={0};
                char new_variable[256]={0};
                while(i!=0 && code[code_len - i]!=')')
                {
                    tree_append_char(new_variable, code[code_len - i]);
                    i = i-1;
                }
                separate(new_variable, sub_buf, sizeof(sub_buf));
                APPEND_TOKEN(sub_buf);

                if(code[code_len - i]!=')')
                {
                    printf("Function is unclosed\n");
                    return;
                }
                i = i-1;
                if(i<=0 || code[code_len - i]!=',')
                {
                    printf("Without a delimiter, it cannot be identified.\n");
                    return;
                }
                i = i-1;
                continue;
            }

            // continue
            if( (i+2 <= code_len) && code[code_len - i +1]=='o' && code[code_len - i +2]=='n' )
            {
                APPEND_TOKEN("continue");
                i = i -8;
                if(i<=0 || code[code_len - i]!=',')
                {
                    printf("Without a delimiter, it cannot be identified.\n");
                    return;
                }
                i = i -1;
                continue;
            }
        }//end if(ch == 'c')

        if(ch == 'd')
        {
            // def
            APPEND_TOKEN("def");
            i = i -3;

            char new_variable[256] = {0};
            //收集def后的函数名(A‑E)
            while( i !=0 && !( code[code_len - i] == 'A' || code[code_len - i] == 'B' || code[code_len - i] == 'C' || code[code_len - i] == 'D' || code[code_len - i] == 'E' ) )
            {
                tree_append_char(new_variable, code[code_len - i]);
                i = i - 1;
            }
            if(i <= 0)
            {
                printf("ERROR:def can not find function name(A‑E)\n");
                return;
            }
            APPEND_TOKEN(new_variable);

            if( i<=0 || code[code_len - i] != '(' )
            {
                printf("The function has no (, can't be recognized\n");
                return;
            }
            APPEND_TOKEN("(");
            i = i - 1;

            //读取形参直到')'
            char param_buf[256]={0};
            while( i!=0 && code[code_len - i] != ')' )
            {
                tree_append_char(param_buf, code[code_len - i]);
                i = i -1;
            }
            APPEND_TOKEN(param_buf);
            APPEND_TOKEN(")");
            i = i -1;

            if(i<=0 || code[code_len - i] != '{')
            {
                printf("There are unclosed curly braces. --def\n");
                return;
            }
            //跳过 { ... } 块内容，一直读到 }
            char new_variable_char;
            while( i!=0 && code[code_len - i] != '}' )
            {
                new_variable_char = code[code_len - i];
                i = i - 1;
            }
            if(i <=0)
            {
                printf("ERROR: def missing '}'\n");
                return;
            }
            i = i - 1;
            continue;
        }//end if(ch == 'd')

        if(ch == 'e')
        {
            // else
            if( (i+3 <= code_len) && code[code_len - i +1]=='l' && code[code_len - i +2]=='s' && code[code_len - i +3]=='e' )
            {
                APPEND_TOKEN("else");
                i = i - 4;
                if(i<=0 || code[code_len - i] != '{')
                {
                    printf("There are unclosed curly braces. --def\n");
                    return;
                }
                char new_variable_char;
                while(i!=0 && code[code_len - i] != '}')
                {
                    new_variable_char = code[code_len - i];
                    i = i - 1;
                }
                if(i <= 0)
                {
                    printf("ERROR: else missing '}'\n");
                    return;
                }
                i = i - 1;
                continue;
            }

            // erf
            if( (i+2 <= code_len) && code[code_len - i +1]=='r' && code[code_len - i +2]=='f' )
            {
                APPEND_TOKEN("erf");
                i = i - 3;
                if(i<=0 || code[code_len - i] != '(')
                {
                    printf("Translation function has an unclosed bracket.\n");
                    return;
                }
                i = i - 1;
                char sub_buf[256]={0};
                char new_variable[256]={0};
                while(i!=0 && code[code_len - i] != ')')
                {
                    tree_append_char(new_variable, code[code_len - i]);
                    i = i - 1;
                }
                separate(new_variable, sub_buf, sizeof(sub_buf));
                APPEND_TOKEN(sub_buf);

                if(code[code_len - i] != ')')
                {
                    printf("Function is unclosed\n");
                    return;
                }
                i = i - 1;
                if(i<=0 || code[code_len - i] != ',')
                {
                    printf("Without a delimiter, it cannot be identified.\n");
                    return;
                }
                i = i - 1;
                continue;
            }

            // exp
            if( (i+2 <= code_len) && code[code_len - i +1]=='x' && code[code_len - i +2]=='p' )
            {
                APPEND_TOKEN("exp");
                i = i - 3;
                if(i<=0 || code[code_len - i] != '(')
                {
                    printf("Translation function has an unclosed bracket.\n");
                    return;
                }
                i = i - 1;
                char sub_buf[256]={0};
                char new_variable[256]={0};
                while(i!=0 && code[code_len - i] != ')')
                {
                    tree_append_char(new_variable, code[code_len - i]);
                    i = i - 1;
                }
                separate(new_variable, sub_buf, sizeof(sub_buf));
                APPEND_TOKEN(sub_buf);

                if(code[code_len - i] != ')')
                {
                    printf("Function is unclosed\n");
                    return;
                }
                i = i - 1;
                if(i<=0 || code[code_len - i] != ',')
                {
                    printf("Without a delimiter, it cannot be identified.\n");
                    return;
                }
                i = i - 1;
                continue;
            }
        }//end if(ch == 'e')

        if(ch == 'f')
        {
            // factorial
            if( (i+8 <= code_len) && code[code_len - i +1]=='a' && code[code_len - i +2]=='c' && code[code_len - i +3]=='t' )
            {
                APPEND_TOKEN("factorial");
                i = i - 9;
                if(i<=0 || code[code_len - i] != '(')
                {
                    printf("Translation function has an unclosed bracket.\n");
                    return;
                }
                i = i - 1;
                char sub_buf[256]={0};
                char new_variable[256]={0};
                while(i!=0 && code[code_len - i] != ')')
                {
                    tree_append_char(new_variable, code[code_len - i]);
                    i = i - 1;
                }
                separate(new_variable, sub_buf, sizeof(sub_buf));
                APPEND_TOKEN(sub_buf);

                if(code[code_len - i] != ')')
                {
                    printf("Function is unclosed\n");
                    return;
                }
                i = i - 1;
                if(i<=0 || code[code_len - i] != ',')
                {
                    printf("Without a delimiter, it cannot be identified.\n");
                    return;
                }
                i = i - 1;
                continue;
            }

            // floor
            if( (i+4 <= code_len) && code[code_len - i +1]=='l' && code[code_len - i +2]=='o' && code[code_len - i +3]=='o' )
            {
                APPEND_TOKEN("floor");
                i = i - 5;
                if(i<=0 || code[code_len - i] != '(')
                {
                    printf("Translation function has an unclosed bracket.\n");
                    return;
                }
                i = i - 1;
                char sub_buf[256]={0};
                char new_variable[256]={0};
                while(i!=0 && code[code_len - i] != ')')
                {
                    tree_append_char(new_variable, code[code_len - i]);
                    i = i - 1;
                }
                separate(new_variable, sub_buf, sizeof(sub_buf));
                APPEND_TOKEN(sub_buf);

                if(code[code_len - i] != ')')
                {
                    printf("Function is unclosed\n");
                    return;
                }
                i = i - 1;
                if(i<=0 || code[code_len - i] != ',')
                {
                    printf("Without a delimiter, it cannot be identified.\n");
                    return;
                }
                i = i - 1;
                continue;
            }
        }//end if(ch == 'f')

        if(ch == 'F')
        {
            APPEND_TOKEN("False");
            i = i - 5;
            continue;
        }

        if(ch == 'g')
        {
            // gcd
            if( (i+2 <= code_len) && code[code_len - i +1]=='c' && code[code_len - i +2]=='d' )
            {
                APPEND_TOKEN("gcd");
                i = i - 3;
                if(i<=0 || code[code_len - i] != '(')
                {
                    printf("Translation function has an unclosed bracket.\n");
                    return;
                }
                i = i - 1;
                char sub_buf[256]={0};
                char new_variable[256]={0};
                while(i!=0 && code[code_len - i] != ')')
                {
                    tree_append_char(new_variable, code[code_len - i]);
                    i = i - 1;
                }
                separate(new_variable, sub_buf, sizeof(sub_buf));
                APPEND_TOKEN(sub_buf);

                if(code[code_len - i] != ')')
                {
                    printf("Function is unclosed\n");
                    return;
                }
                i = i - 1;
                if(i<=0 || code[code_len - i] != ',')
                {
                    printf("Without a delimiter, it cannot be identified.\n");
                    return;
                }
                i = i - 1;
                continue;
            }
            // gpu
            if( (i+2 <= code_len) && code[code_len - i +1]=='p' && code[code_len - i +2]=='u' )
            {
                APPEND_TOKEN("gpu");
                i = i - 3;
                if(i<=0 || code[code_len - i] != '(')
                {
                    printf("Translation function has an unclosed bracket.\n");
                    return;
                }
                i = i - 1;
                char sub_buf[256]={0};
                char new_variable[256]={0};
                while(i!=0 && code[code_len - i] != ')')
                {
                    tree_append_char(new_variable, code[code_len - i]);
                    i = i - 1;
                }
                separate(new_variable, sub_buf, sizeof(sub_buf));
                APPEND_TOKEN(sub_buf);

                if(code[code_len - i] != ')')
                {
                    printf("Function is unclosed\n");
                    return;
                }
                i = i - 1;
                if(i<=0 || code[code_len - i] != ',')
                {
                    printf("Without a delimiter, it cannot be identified.\n");
                    return;
                }
                i = i - 1;
                continue;
            }
        }//end if(ch == 'g')

        if(ch == 'i')
        {
            // if
            if( (i+1 <= code_len) && code[code_len - i +1]=='f' )
            {
                APPEND_TOKEN("if");
                i = i - 2;
                if(i<=0 || code[code_len - i] != '(')
                {
                    printf("The function doesn't have (\n");
                    return;
                }
                i = i - 1;

                char cond_buf[256]={0};
                while(i!=0 && code[code_len - i] != ')')
                {
                    tree_append_char(cond_buf, code[code_len - i]);
                    i = i - 1;
                }
                char cond_sub[256]={0};
                separate(cond_buf, cond_sub, sizeof(cond_sub));
                APPEND_TOKEN(cond_sub);
                APPEND_TOKEN(")");

                i = i - 1;
                if(i<=0 || code[code_len - i] != '{')
                {
                    printf("There are unclosed curly braces. --def\n");
                    return;
                }
                char new_variable_char;
                while(i!=0 && code[code_len - i] != '}')
                {
                    new_variable_char = code[code_len - i];
                    i = i - 1;
                }
                if(i <=0)
                {
                    printf("ERROR:if missing '}'\n");
                    return;
                }
                i = i - 1;
                continue;
            }
            // in
            else
            {
                APPEND_TOKEN("in");
                i = i - 3;
                continue;
            }
        }//end if(ch == 'i')

        if(ch == 'l')
        {
            // lcm
            if( (i+2 <= code_len) && code[code_len - i +1]=='c' && code[code_len - i +2]=='m' )
            {
                APPEND_TOKEN("lcm");
                i = i - 3;
                if(i<=0 || code[code_len - i] != '(')
                {
                    printf("Translation function has an unclosed bracket.\n");
                    return;
                }
                i = i - 1;
                char sub_buf[256]={0};
                char new_variable[256]={0};
                while(i!=0 && code[code_len - i] != ')')
                {
                    tree_append_char(new_variable, code[code_len - i]);
                    i = i - 1;
                }
                separate(new_variable, sub_buf, sizeof(sub_buf));
                APPEND_TOKEN(sub_buf);

                if(code[code_len - i] != ')')
                {
                    printf("Function is unclosed\n");
                    return;
                }
                i = i - 1;
                if(i<=0 || code[code_len - i] != ',')
                {
                    printf("Without a delimiter, it cannot be identified.\n");
                    return;
                }
                i = i - 1;
                continue;
            }
            // log
            if( (i+2 <= code_len) && code[code_len - i +1]=='o' && code[code_len - i +2]=='g' )
            {
                APPEND_TOKEN("log");
                i = i - 3;
                if(i<=0 || code[code_len - i] != '(')
                {
                    printf("Translation function has an unclosed bracket.\n");
                    return;
                }
                i = i - 1;
                char sub_buf[256]={0};
                char new_variable[256]={0};
                while(i!=0 && code[code_len - i] != ')')
                {
                    tree_append_char(new_variable, code[code_len - i]);
                    i = i - 1;
                }
                separate(new_variable, sub_buf, sizeof(sub_buf));
                APPEND_TOKEN(sub_buf);

                if(code[code_len - i] != ')')
                {
                    printf("Function is unclosed\n");
                    return;
                }
                i = i - 1;
                if(i<=0 || code[code_len - i] != ',')
                {
                    printf("Without a delimiter, it cannot be identified.\n");
                    return;
                }
                i = i - 1;
                continue;
            }
        }//end if(ch == 'l')

        if(ch == 'N')
        {
            // None
            if( (i+4 <= code_len) && code[code_len - i +1]=='o' && code[code_len - i +2]=='n' && code[code_len - i +3]=='e' )
            {
                APPEND_TOKEN("None");
                i = i - 5;
                continue;
            }
        }//end if(ch == 'N')

        if(ch == 'n')
        {
            // not
            if( (i+2 <= code_len) && code[code_len - i +1]=='o' && code[code_len - i +2]=='t' )
            {
                APPEND_TOKEN("not");
                i = i - 4;
                continue;
            }
        }//end if(ch == 'n')

        if(ch == 'o')
        {
            // or
            if( (i+1 <= code_len) && code[code_len - i +1]=='r' )
            {
                APPEND_TOKEN("or");
                i = i - 2;
                continue;
            }
        }//end if(ch == 'o')

        if(ch == 'r')
        {
            // rand / randint
            if( (i+3 <= code_len) && code[code_len - i +1]=='a' && code[code_len - i +2]=='n' && code[code_len - i +3]=='d' )
            {
                if( (i+6 <= code_len) && code[code_len - i +4]=='i' && code[code_len - i +5]=='n' && code[code_len - i +6]=='t' )
                {
                    APPEND_TOKEN("randint");
                    i = i - 7;
                }
                else
                {
                    APPEND_TOKEN("rand");
                    i = i - 4;
                }
                if(i<=0 || code[code_len - i] != '(')
                {
                    printf("Translation function has an unclosed bracket.\n");
                    return;
                }
                i = i - 1;
                char sub_buf[256]={0};
                char new_variable[256]={0};
                while(i!=0 && code[code_len - i] != ')')
                {
                    tree_append_char(new_variable, code[code_len - i]);
                    i = i - 1;
                }
                separate(new_variable, sub_buf, sizeof(sub_buf));
                APPEND_TOKEN(sub_buf);

                if(code[code_len - i] != ')')
                {
                    printf("Function is unclosed\n");
                    return;
                }
                i = i - 1;
                if(i<=0 || code[code_len - i] != ',')
                {
                    printf("Without a delimiter, it cannot be identified.\n");
                    return;
                }
                i = i - 1;
                continue;
            }

            // return
            if( (i+5 <= code_len)
                && code[code_len - i +1]=='e'
                && code[code_len - i +2]=='t'
                && code[code_len - i +3]=='u'
                && code[code_len - i +4]=='r'
                && code[code_len - i +5]=='n' )
            {
                APPEND_TOKEN("return");
                i = i - 7;
                continue;
            }
        }//end if(ch == 'r')

        if(ch == 's')
        {
            // sin / sinh
            if( (i+3 <= code_len) && code[code_len - i +1]=='i' && code[code_len - i +2]=='n' )
            {
                if( (i+4 <= code_len) && code[code_len - i +3]=='h' )
                {
                    APPEND_TOKEN("sinh");
                    i = i - 4;
                }
                else
                {
                    APPEND_TOKEN("sin");
                    i = i - 3;
                }
                if(i<=0 || code[code_len - i] != '(')
                {
                    printf("Translation function has an unclosed bracket.\n");
                    return;
                }
                i = i - 1;
                char sub_buf[256]={0};
                char new_variable[256]={0};
                while(i!=0 && code[code_len - i] != ')')
                {
                    tree_append_char(new_variable, code[code_len - i]);
                    i = i - 1;
                }
                separate(new_variable, sub_buf, sizeof(sub_buf));
                APPEND_TOKEN(sub_buf);

                if(code[code_len - i] != ')')
                {
                    printf("Function is unclosed\n");
                    return;
                }
                i = i - 1;
                if(i<=0 || code[code_len - i] != ',')
                {
                    printf("Without a delimiter, it cannot be identified.\n");
                    return;
                }
                i = i - 1;
                continue;
            }
            // sqrt
            if( (i+3 <= code_len) && code[code_len - i +1]=='q' && code[code_len - i +2]=='r' && code[code_len - i +3]=='t' )
            {
                APPEND_TOKEN("sqrt");
                i = i - 4;
                if(i<=0 || code[code_len - i] != '(')
                {
                    printf("Translation function has an unclosed bracket.\n");
                    return;
                }
                i = i - 1;
                char sub_buf[256]={0};
                char new_variable[256]={0};
                while(i!=0 && code[code_len - i] != ')')
                {
                    tree_append_char(new_variable, code[code_len - i]);
                    i = i - 1;
                }
                separate(new_variable, sub_buf, sizeof(sub_buf));
                APPEND_TOKEN(sub_buf);

                if(code[code_len - i] != ')')
                {
                    printf("Function is unclosed\n");
                    return;
                }
                i = i - 1;
                if(i<=0 || code[code_len - i] != ',')
                {
                    printf("Without a delimiter, it cannot be identified.\n");
                    return;
                }
                i = i - 1;
                continue;
            }
        }//end if(ch == 's')

        if(ch == 't')
        {
            // tan / tanh
            if( (i+3 <= code_len) && code[code_len - i +1]=='a' && code[code_len - i +2]=='n' )
            {
                if( (i+4 <= code_len) && code[code_len - i +3]=='h' )
                {
                    APPEND_TOKEN("tanh");
                    i = i - 4;
                }
                else
                {
                    APPEND_TOKEN("tan");
                    i = i - 3;
                }
                if(i<=0 || code[code_len - i] != '(')
                {
                    printf("Translation function has an unclosed bracket.\n");
                    return;
                }
                i = i - 1;
                char sub_buf[256]={0};
                char new_variable[256]={0};
                while(i!=0 && code[code_len - i] != ')')
                {
                    tree_append_char(new_variable, code[code_len - i]);
                    i = i - 1;
                }
                separate(new_variable, sub_buf, sizeof(sub_buf));
                APPEND_TOKEN(sub_buf);

                if(code[code_len - i] != ')')
                {
                    printf("Function is unclosed\n");
                    return;
                }
                i = i - 1;
                if(i<=0 || code[code_len - i] != ',')
                {
                    printf("Without a delimiter, it cannot be identified.\n");
                    return;
                }
                i = i - 1;
                continue;
            }
        }//end if(ch == 't')

        if(ch == 'T')
        {
            // True
            if( (i+4 <= code_len) && code[code_len - i +1]=='r' && code[code_len - i +2]=='u' && code[code_len - i +3]=='e' )
            {
                APPEND_TOKEN("True");
                i = i - 5;
                continue;
            }
        }//end if(ch == 'T')

        if(ch == 'w')
        {
            // while
            if( (i+4 <= code_len) && code[code_len - i +1]=='h' && code[code_len - i +2]=='i' && code[code_len - i +3]=='l' && code[code_len - i +4]=='e' )
            {
                APPEND_TOKEN("while");
                i = i - 5;
                if(i<=0 || code[code_len - i] != '(')
                {
                    printf("The function doesn't have (\n");
                    return;
                }
                i = i - 1;

                char cond_buf[256]={0};
                while(i!=0 && code[code_len - i] != ')')
                {
                    tree_append_char(cond_buf, code[code_len - i]);
                    i = i - 1;
                }
                char cond_sub[256]={0};
                separate(cond_buf, cond_sub, sizeof(cond_sub));
                APPEND_TOKEN(cond_sub);
                APPEND_TOKEN(")");

                i = i - 1;
                if(i<=0 || code[code_len - i] != '{')
                {
                    printf("There are unclosed curly braces. --while\n");
                    return;
                }
                char new_variable_char;
                while(i!=0 && code[code_len - i] != '}')
                {
                    new_variable_char = code[code_len - i];
                    i = i - 1;
                }
                if(i <=0)
                {
                    printf("ERROR: while missing '}'\n");
                    return;
                }
                i = i - 1;
                continue;
            }
        }//end if(ch == 'w')

        // -------- 数字解析 --------
        if(ch >= '0' && ch <= '9')
        {
            char num_buf[256]={0};
            while(i != 0 && code[code_len - i] >= '0' && code[code_len - i] <= '9')
            {
                tree_append_char(num_buf, code[code_len - i]);
                i--;
            }
            APPEND_TOKEN(num_buf);
            continue;
        }

        // -------- 运算符与符号；处理双字符 >=  <= --------
        if(ch == '+'){ APPEND_TOKEN("+"); i--; continue; }
        if(ch == '-'){ APPEND_TOKEN("-"); i--; continue; }
        if(ch == '*'){ APPEND_TOKEN("*"); i--; continue; }
        if(ch == '/'){ APPEND_TOKEN("/"); i--; continue; }

        if(ch == '>')
        {
            if( i>0 && (code_len - i +1) < code_len && code[code_len - i +1] == '=' )
            {
                APPEND_TOKEN(">=");
                i -= 2;
            }
            else
            {
                APPEND_TOKEN(">");
                i--;
            }
            continue;
        }
        if(ch == '<')
        {
            if( i>0 && (code_len - i +1) < code_len && code[code_len - i +1] == '=' )
            {
                APPEND_TOKEN("<=");
                i -= 2;
            }
            else
            {
                APPEND_TOKEN("<");
                i--;
            }
            continue;
        }

        // 空格跳过
        if(ch == ' ')
        {
            i--;
            continue;
        }

        //兜底：未识别字符，向前移动，防止死循环
        i = i - 1;
    }// while(i!=0) 循环结束
#undef APPEND_TOKEN
}

#endif
