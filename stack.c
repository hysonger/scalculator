// 一个简单的链栈实现 Songer
#include <stdlib.h>
#include <stdio.h>
#include "stack.h"
#include "scalc.h"

char stack_popup(struct Stack *s, union Item *data){
    if(s->length <= 0){
        die("ERROR: stack is empty and unable to popup");
        return -1;
    }

    struct StackItem *p = s->top;
    char ret = s->top->type;
    *data = s->top->data;
    s->top = s->top->next;
    free(p); s->length--;

    return ret;
}

int stack_pushin(struct Stack *s, union Item data, char type){
    struct StackItem *p = (struct StackItem *)malloc(sizeof(struct StackItem));
    if(!p){
        die("ERROR: unable to allocate memory space in pushing stack");
    }

    p->data = data; p->type = type;
    p->next = s->top;
    s->top = p;
    s->length++;
    
    return 0;
}

void stack_destroy(struct Stack *s){
    union Item trashbin;
    for(int i = 0; i < s->length; ++i){
        stack_popup(s, &trashbin);
    }
}

void stack_print(struct Stack *s){
    struct StackItem *p = s->top;
    for(int i = 0; i < s->length; ++i){
        switch(p->type){
            case 'c': putchar(p->data.c); break;
            case 'i': printf("%d ", p->data.i); break;
            case 'd': printf("%f ", p->data.d); break;
            default: die("ERROR: Invaild symbol in stack_print()");
        }
        p = p->next;
    }
    putchar('\n');
}