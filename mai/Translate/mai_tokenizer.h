
#include <stdio.h>
#include <string.h>

// 键值对结构
typedef struct {
    char *key;      // 关键字（英文）
    char *code;     // 对应的数字编码
} MAI_Keyword;

//---------config---------
//变量长度为4,和列表
//------------------------

// MAI语言完整对照表
MAI_Keyword mai_dict[] = {
    // 流程控制关键字
    {"if", "01"},
    {"for", "02"},
    {"else", "03"},
    {"while", "04"},
    
    // 运算符
    {"+", "05"},
    {"-", "06"},
    {"*", "07"},
    {"/", "08"},
    {"%", "36"},
    
    // 数学函数（去掉math.前缀）
    {"sqrt", "09"},
    {"pow", "10"},
    {"exp", "11"},
    {"log", "12"},
    {"log10", "14"},
    {"log2", "15"},
    {"sin", "16"},
    {"cos", "17"},
    {"tan", "18"},
    {"asin", "19"},
    {"acos", "20"},
    {"atan", "21"},
    {"gcd", "22"},
    {"lcm", "23"},
    {"abs", "40"},
    {"e", "41"},
    {"pi", "42"},
    {"factorial", "43"},
    {"ceil", "45"},
    {"floor", "46"},//注意
    
    // 随机函数（去掉random.前缀）
    {"random", "47"},
    {"randint", "48"},
    
    // 逻辑和比较运算符
    {"in", "25"},
    {"=", "26"},
    {"==", "27"},
    {">=", "28"},
    {"<=", "29"},
    {"<", "30"},
    {">", "31"},
    {"!=", "39"},
    {"and", "34"},
    {"or", "35"},
    {"not", "49"},
    
    // 括号和列表操作
    {"list_access", "32"},
    {"create", "33"},
    {"(", "37"},
    {")", "38"},
    {"print", "52"},     // 补充
    {"def", "53"},       // 补充
    {"return", "54"},    // 补充
    {"break", "55"},     // 补充
    {"continue", "56"},  // 补充
    {"True", "50"},
    {"False", "57"},     // 补充
};

// 计算字典大小（总元素个数 / 每个元素大小）
int mai_dict_len = sizeof(mai_dict) / sizeof(mai_dict[0]);

// 查找函数：根据关键字返回编码
char* found_command(char *duixiang) {
    for (int i = 0; i < mai_dict_len; i++) {
        if (strcmp(mai_dict[i].key, duixiang) == 0) {
            return mai_dict[i].code;
        }
    }
    return NULL;  // 未找到返回NULL
}
char* found_number(char *code) {
    for (int i = 0; i < mai_dict_len; i++) {
        if (strcmp(mai_dict[i].code, code) == 0) {
            return mai_dict[i].key;  // 返回关键字
        }
    }
    return NULL;  // 未找到返回NULL
}


char* number_code(char *number);
char* number_code(char *number){
    int le=strlen(number);
    int start_le=le;
    char *result;
    while(le!=0){
        if (number[start_le-le]=='0'){
           //判定为命令
           le=le-1;
           char command_number=number[(start_le-le)]+number[(start_le-le)+1]+",";
           int le=le-2 ;
           result=result+found_command(command_number);
           
        }
        if(number[start_le-le]=='1'){
           //判定为变量
           le =le-1;
           char command_number=number[start_le-le]+number[(start_le-le)+1]+number[(start_le-le)+2]+number[(start_le-le)+3]+number[(start_le-le)+4]+","
           int le=le-4;
           result=result+command_number;
           
        }
        if(number[start_le-le]=='2'){
           //判定为常数,
           le = le-1;
           int number_leaght=number[start_le-le]int();
           while(number_leaght==0){
             number_leaght=number_leaght-1;
             char number_result=number_result+number[start_le-le];
             int le = le-1;
           }
           result=result+number_result+','
        }
        else{
            printf("Error:程序有误\n");
        }
    }
    return result;
}
char* code_number(char *code);
char* code_number(char *code){
    int le=strlen(code);
    int start_le=le;
    char result;
    char lingshi_result;
    while(le!=0){
        char test=code[start_le-le];
        if(test > 47 && test < 58){
            //判定为数字变量列表
            if(code[start_le-le]=='0'){
                //判定为数字
                le=le-1
                char* lingshi_number;
                int i=0;//初始化常数计算器
                while(test > 47 && test < 58){
                    le=le-1;
                    char test=code[start_le-le];
                    lingshi_number=lingshi_number+test
                }
                result=result+'2'+lingshi_number;
            }
        else{
            //判定为变量或列表
            if(code[start_le-le]=='0'){
                //判定为变量
                char lingshi_bianliang;
                le=le-1;
                for(int i=0;i<4,i++){
                    le=le-1;
                    lingshi_bianliang=lingshi_bianliang+code[start_le-le];
                }
                result=result+"1"+lingshi_bianliang;
            }
            else{
                //判定为列表
                char lingshi_bianliang;
                le=le-1
                for(int i=0;i<4,i++){
                    le=le-1;
                    lingshi_bianliang=lingshi_bianliang+code[start_le-le];
                }
                result=result+'2'+lingshi_bianliang;
            }
          }
            
        }
        else{
            //其他情况
            while(found_number(lingshi_result)!=NULL){
               //查找命令
               le=le-1;
               lingshi_result=lingshi_result+code[start_le-le];
            }
            //----------特殊情况------------
            if(lingshi_result=='='){
                if(code[(start_le-le)+1]=='='){
                    int le = le-1;
                    lingshi_result=="==";
                }
                if(code[(start_le-le)+1]=='<'){
                    int le = le-1;
                    lingshi_result=="<=";
                }
                if(code[(start_le-le)+1]=='>'){
                    int le = le-1;
                    lingshi_result==">=";
                }
                result=result+'0'+lingshi_result;
            }
            else{
                result=result+'0'+lingshi_result;
            }
            return result;
        }
        
    }
}
