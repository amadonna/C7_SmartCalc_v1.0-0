#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

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
    prior_t prior;
    type_sign t_val;
    struct Node *next;
} stack_symbol;

typedef struct Num
{
    double val;
    struct Num *next;
} stack_num;


int parser(char *str, stack_symbol **st);
void push_sym(stack_symbol **st, prior_t pr, type_sign sign);
stack_symbol *get_sym(stack_symbol **st);
stack_num *get_num(stack_num **st);
int digits(char c);
double to_double(char *str, int *i);


void push_sym(stack_symbol **st, prior_t pr, type_sign sign) {
    stack_symbol *st_temp = calloc(1, sizeof(stack_symbol));
    st_temp->t_val = sign;
    st_temp->prior = pr;
    st_temp->next = *st;
    *st = st_temp;
}

void push_num(stack_num **st, double num) {
    stack_num *st_temp = calloc(1, sizeof(stack_symbol));
    st_temp->val = num;
    st_temp->next = *st;
    *st = st_temp;
}

stack_symbol *get_sym(stack_symbol **st) {
    stack_symbol *temp = NULL;
    stack_symbol *ret = calloc(1, sizeof(stack_symbol));
    temp = *st;
    ret->prior = temp->prior;
    ret->t_val = temp->t_val;
    *st = (*st)->next;
    free(temp);
    return ret;
}

stack_num *get_num(stack_num **st) {
    stack_num *temp = NULL;
    stack_num *ret = calloc(1, sizeof(stack_symbol));
    temp = *st;
    ret->val = temp->val;
    *st = (*st)->next;
    free(temp);
    return ret;
}


int main() {
    stack_num *st = NULL;
    stack_symbol *temp = NULL;
    st = calloc(4, sizeof(stack_symbol));
    temp = calloc(4, sizeof(stack_symbol));
    for (int i = 0; i < 4; i++) {
        push_sym(&temp,  i, i);
        push_num(&st, (double)i);
    }

    for (int j = 0; j < 4; j++) {
        printf("pr = %d\ttype = %d", temp->prior, temp->t_val);
        printf("\t\tpr = %lf\n", st->val);
        get_sym(&temp);
        get_num(&st);
    }
    free(st);
    free(temp);
    return 0;
}


int to_stack(char *str, stack_symbol **st, stack_num **num) {
    int ret = 0;
    for(int i = 0; str[i] != '\0'; i++) {
        if(isspace(str[i]))
            i++;
        if(digits(str[i]) == 1) {
            push_num(num, to_double(str, &i));
        } else if()
        
    }
    return ret;
}

double to_double(char *str, int *i) {
    double res = 0;
    int j = 0;
    char temp[32] = {'\0'};
    while((digits(str[*i]) == 1) || (str[*i] == '.' && digits(str[*i - 1]) == 1 && digits(str[*i + 1]) == 1)) {
        temp[j] = str[*i];
        j++;
        *i += 1;
    }
    res = atof(temp);
    return res;
}

int digits(char c) {
    int ret = 0;
    if(c > '0' && c < '9')
        ret = 1;
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