// 定义一个链栈类型
#ifndef _stack_h
#define _stack_h

union Item{
    char c;
    int i;
    double d;
};

struct StackItem{
    union Item data;
    char type; // c i f d
    struct StackItem *next;
};

struct Stack{
    unsigned int length;
    struct StackItem *top;
};

char stack_popup(struct Stack *s, union Item *data);
int stack_pushin(struct Stack *s, union Item data, char type);
void stack_destroy(struct Stack *s);
void stack_print(struct Stack *s);

#endif