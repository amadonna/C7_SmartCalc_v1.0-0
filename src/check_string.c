#include<stdio.h>

#include <stdlib.h>

typedef enum {
    x,
    number,
    dot,
    sum,
    del,
    mul,
    divi,
    mod,
    pow,
    sin,
    cos,
    tan,
    asin,
    acos,
    atan,
    sqrt,
    ln,
    log,
    openB,
    closeB
} type_sign;

typedef enum {
    sh,
    mid,
    un,
    high,
    brac
} prior_t;

typedef struct Node
{
    double val;
    prior_t prior;
    type_sign t_val;
    struct Node *next;
} stack_21;

void push(stack_21 **st, double num, prior_t pr, type_sign sign) {
    stack_21 *st_temp = calloc(1, sizeof(stack_21));
    st_temp->val = num;
    st_temp->t_val = sign;
    st_temp->prior = pr;
    st_temp->next = *st;
    *st = st_temp;
}

stack_21 *get(stack_21 **st) {
    stack_21 *temp = NULL;
    stack_21 *ret = calloc(1, sizeof(stack_21));
    temp = *st;
    ret->val = temp->val;
    ret->prior = temp->prior;
    ret->t_val = temp->t_val;
    *st = (*st)->next;
    return ret;
}


int main() {
    return 0;
}

int parser(char *str) {
    int ret = 0;
    stack_21 *st = NULL;
    return ret;
}

/*
int checker(char *str) {
    int ret = 0;
    int stop = 0;
    for(int i = 0; str[i] + '\0' && stop == 0; i++) {

    }
    return ret;
}
*/