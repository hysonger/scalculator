#include "stack.h"

#define BUFFER_MAX 64

void die(char *message);

short get_prior(char a, short flag);

void popup_calc(struct Stack *nums, struct Stack *symbols, char n);

double parse_expression(char *str, int length);

