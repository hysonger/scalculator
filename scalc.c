#include "scalc.h"
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

// 报错函数
void die(char *message){
    puts(message); exit(-1);
}


// 判断运算符优先级。注意栈内外优先级需区分考虑，flag=0 栈内 flag=1 栈外
short get_prior(char a, short flag){ 
    if(a == '#') return 0;

    if(flag){
        switch(a){
            case '(': return 6;
            case '+': case '-': return 2;
            case '*': case '/': return 4;
            case ')': return 1;
            default:
                die("ERROR: Invaild symbol"); return 0;
        }
    }else{
        switch(a){
            case '(': return 1;
            case '+': case '-': return 3;
            case '*': case '/': return 5;
            case ')': return 6;
            default:
                die("ERROR: Invaild symbol"); return 0;
        }
    }
}


// 判断当前运算符，并比较优先级，进行所需的出栈操作
void popup_calc(struct Stack *nums, struct Stack *symbols, char n){
    double a, b;
    union Item da = {}, db = {}, dsymbol = {};
    union Item dresult = {};


    while(symbols->top && nums->top && symbols->length >= 1 && nums->length >= 2 
    && get_prior(symbols->top->data.c, 0) >= get_prior(n, 1)){ 
        // 栈不空，元素足够弹出，且新运算符优先级小于等于栈顶
        stack_popup(symbols, &dsymbol); // 则弹出一个符号看看
        
        if(dsymbol.c == '('){ // ( 不参与运算，跳过
            continue;
        }
        
        // 弹出两个操作数
        switch(stack_popup(nums, &da)){
            case 'd': a = da.d; break;
            case 'i': a = da.i; break;
            default: 
                die("ERROR: Invaild type in calc()");
        }
        switch(stack_popup(nums, &db)){
            case 'd': b = db.d; break;
            case 'i': b = db.i; break;
            default: 
                die("ERROR: Invaild type in calc()");
        }

        printf("[OPT]%f and %f is doing %c\n", b, a, dsymbol.c);

        switch(dsymbol.c){ // 计算
            case '+': dresult.d =  b + a; break;
            case '-': dresult.d =  b - a; break;
            case '*': dresult.d =  b * a; break;
            case '/': 
                //assert(abs(b - 0) < 0.000001);
                dresult.d = b / a; break;
            case '(': break;
            default: 
                die("ERROR: Invaild symbol in calc()");
        }
        stack_pushin(nums, dresult, 'd'); // 得到的新结果入栈
    }
}


// 缓冲区数值入栈
void pushin_buffer(struct Stack *nums, char *buffer, int buffer_len, short flag_point){
    union Item data={};
    
    if(buffer_len > BUFFER_MAX - 1){
        die("ERROR: too long number!");
    }
    buffer[buffer_len] = '\0'; // 严防字符串无结尾！

    if(buffer_len > 0){
        if(flag_point){
            data.d = atof(buffer);
            stack_pushin(nums, data, 'd');
        }else{
            data.i = atoi(buffer);
            stack_pushin(nums, data, 'i');
        }
            printf("[NUM]%s is pushing in: ", buffer);
            stack_print(nums);
    }
}


// 计算表达式字符串并返回结果
double parse_expression(char *str, int length){
    struct Stack nums = {}; // 数栈
    struct Stack symbols = {}; // 符号栈

    int i = 0; // for用
    char buffer[BUFFER_MAX] = {}; // 存放连续的数值
    int buffer_len = 0; // buffer的长度
    short flag_point = 0; // 检测数字时，是否已检测到点（有点则为浮点型）
    char prev = '('; // 这个初值是为了捕获第一个字符即为正负号的特殊情况
    union Item data = {}; // 临时存放新栈项数据


    for(i = 0; i < length && str[i] != '\0'; ++i){
        if((str[i] == '+' || str[i] == '-') && 
        (prev == '(' || prev == '*' || prev == '/' || prev == '+' || prev == '-') && 
        buffer_len <= 0){ 
            // 特殊情况：含义为正负号而非加减的+和-
            buffer[0] = str[i];
            buffer_len = 1;
            continue;
        }else if((str[i] >= '0' && str[i] <= '9') || str[i] == '.'){ // 数字，小数点
            if(buffer_len > BUFFER_MAX - 2){
                die("ERROR: too long number!");
            }
            buffer[buffer_len] = str[i];
            buffer_len++;
            if(str[i] == '.'){
                if(!flag_point){ // 出现两次点号则报错
                    flag_point = 1;
                }else{
                    die("ERROR: Incorrect input, more than one point");
                }
            }
        }else if(str[i] == ' '){
            continue; // 空格忽略
        }else if(buffer_len > 0 || str[i] == '(' || prev == ')'){ // 运算符（注意 -( 和 )+ 这种合法的连续符号情况）
            // 数值入栈
            pushin_buffer(&nums, buffer, buffer_len, flag_point);
            buffer[0] = '\0'; buffer_len = 0; flag_point = 0; // 清退缓冲区内容, flag置0，数值入栈结束

            popup_calc(&nums, &symbols, str[i]); // 比较优先级，出栈和计算

            // 运算符入栈
            if(str[i] != ')'){
                data.c = str[i]; 
                stack_pushin(&symbols, data, 'c');            
                printf("[SYM]%c is pushing in: ", str[i]);
                stack_print(&symbols);
            }
            
            
        }else{
            die("ERROR: Incorrect input, unknown symbol");
        }

        prev = str[i]; // 记录上一个有效字符以便情况判断
    }

    // 最后要确保缓冲区内剩余的数值入栈，重复一次
    pushin_buffer(&nums, buffer, buffer_len, flag_point);


    popup_calc(&nums, &symbols, '#'); // #表示字符串结束，栈的剩余项可全部弹出
    

    // 结束时，最多只应有一个数和一个符号（"("）在栈中
    if(nums.length == 1 && symbols.length < 2){
        stack_popup(&nums, &data);
    }else{
        die("ERROR: Final condition is not correct");
    }

    // 回收内存，返回值
    stack_destroy(&nums);
    stack_destroy(&symbols);
    return data.d;
}

