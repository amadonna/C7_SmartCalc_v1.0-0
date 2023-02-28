#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

typedef enum {
    unar,
    sum,
    del,
    mul,
    divi,
    mod,
    pow,
    coss,
    sinn,
    tann,
    acoss,
    asinn,
    atann,
    sqrtt,
    lnn,
    logg,
    openB,
    closeB
} type_sign;

typedef enum {
    unary,
    open_b,
    close_b,
    plus_minus,
    div_mul_mod,
    expon_funs
} prior_t;

typedef struct Node
{
    prior_t prior;
    type_sign t_val;
    struct Node *next;
} stack_symbol;

/*typedef struct Num
{
    double val;
    struct Num *next;
} stack_num;*/


int parser(char *str, stack_symbol **st);
void push_sym(stack_symbol **st, prior_t pr, type_sign sign);
stack_symbol *get_sym(stack_symbol **st);
//stack_num *get_num(stack_num **st);
int digits(char c);
char* cut_char(char *str, int *i);
int is_func(int *i, char *str);
stack_symbol *watch_peak(stack_symbol *st);
int is_oper(char *str, int *i);
stack_symbol *watch_peak(stack_symbol *st);
int is_empty(stack_symbol *st);
int get_prior(int sign);
int is_unary(char *str, int *i);
int comp_prior(int i, stack_symbol *st);


void push_sym(stack_symbol **st, prior_t pr, type_sign sign) {
    stack_symbol *st_temp = calloc(1, sizeof(stack_symbol));
    st_temp->prior = pr;
    st_temp->t_val = sign;
    st_temp->next = *st;
    *st = st_temp;
}

/*void push_num(stack_num **st, double num) {
    stack_num *st_temp = calloc(1, sizeof(stack_symbol));
    st_temp->val = num;
    st_temp->next = *st;
    *st = st_temp;
}*/

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



stack_symbol *watch_peak(stack_symbol *st) {
    stack_symbol *temp = NULL;
    temp = get_sym(&st);
    push_sym(&temp, temp->prior, temp->t_val);
    return temp;
}

int to_stack(char *str, stack_symbol **st, char **outs) {
    int ret = 0;
    int c_out = 0;
    int check_func = 0;
    int check_oper = 0;
    for(int i = 0; str[i] != '\0'; i++) {
        if(is_unary(str, &i) == 1) {
            if(digits(str[i + 1]) == 1) {
                *outs[c_out] = str[i];
                i++;
            } else
                ret = -1;
        }
        if(digits(str[i]) == 1) {
            strcpy(*outs + strlen(*outs), cut_char(str, &i));
            *outs[strlen(*outs)] = '\t';
        } else if((check_func = is_func(&i, str)) != 0)
            push_sym(st, get_prior(check_func) ,check_func);
        else if((check_oper = is_oper(str, &i))) {
            if(comp_prior(get_prior(check_oper), *st) == 1)

            push_sym(st, get_prior(check_oper), check_oper);
            i += 1;
        }
        
    }
    return ret;
}

void move_to_string(char **out, stack_symbol *st) {

}

int comp_prior(int i, stack_symbol *st) {
    int ret = 0;
    if(i >= watch_peak(st)->prior || is_empty(st))
        ret = 1;
    return ret;
}

int get_prior(int sign) {
    int ret = 0;
    if(sign == unar)
        ret = unary; 
    else if(sign == openB)
        ret = open_b;
    else if(sign == closeB)
        ret = close_b;
    else if(sign == sum || sign == del)
        ret = plus_minus;
    else if(sign == divi || sign == mul || sign == mod)
        ret = div_mul_mod;
    else if(sign >= 6 && sign <= 15)
        ret = expon_funs;
    return ret;
}

int is_unary(char *str, int *i) {
    int ret = 0;
    if((str[*i] == '-' || str[*i] == '+') && (str[*i - 1] != ')' && digits(str[*i]) == 0))
        ret = 1;
    return ret;
}

int is_oper(char *str, int *i) { return (str[*i] == '+' || str[*i] == '-' || str[*i] == '/' || str[*i] == '*' || str[*i] == '%');}

int is_empty(stack_symbol *st) {
    int ret = 0;
    if(st != NULL)
        ret = 1;
    return ret;
}
int is_func(int *i, char *str) {
    int ret = 0;
    if(str[*i] >= 'a' && str[*i] <= 'z') {
        if(strncmp("cos", str + *i, 3) == 0)
            ret = coss;
        else if(strncmp("sin", str + *i, 3) == 0)
            ret = sinn;
        else if(strncmp("tan", str + *i, 3) == 0)
            ret = tann;
        else if(strncmp("acos", str + *i, 4) == 0)
            ret = acoss;
        else if(strncmp("asin", str + *i, 4) == 0)
            ret = asinn;
        else if(strncmp("atan", str + *i, 4) == 0)
            ret = atann;
        else if(strncmp("sqrt", str + *i, 4) == 0)
            ret = sqrtt;
        else if(strncmp("ln", str + *i, 4) == 0)
            ret = lnn;
        else if(strncmp("log", str + *i, 4) == 0)
            ret = logg;
    }
    if(ret == tann || ret == sinn || ret == coss || ret == logg)
        *i += 3;
    else if(ret == acoss || ret == asinn || ret == atann || ret == sqrtt)
        *i += 4;
    else if(ret == lnn)
        *i += 2;
    return ret;
}

char* cut_char(char *str, int *i) {
    char *ret = NULL;
    int j = 0;
    char temp[32] = {'\0'};
    while((digits(str[*i]) == 1) || (str[*i] == '.' && digits(str[*i - 1]) == 1 && digits(str[*i + 1]) == 1)) {
        temp[j] = str[*i];
        j++;
        *i += 1;
    }
    ret = temp;
    return ret;
}

int digits(char c) {
    int ret = 0;
    if(c > '0' && c < '9')
        ret = 1;
    return ret;
        
}


