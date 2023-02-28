#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>


typedef enum {
    mod = 37,
    mul = 42,
    sum = 43,
    del = 45,
    divi = 47,
    pow = 94,
    coss,
    sinn,
    tann,
    acoss,
    asinn,
    atann,
    sqrtt,
    lnn,
    logg,
    openB = '(',
    closeB,
    unar_pos = 124,
    unar_neg = 126
} type_sign;

typedef enum {
    open_b,
    close_b,
    plus_minus,
    div_mul_mod,
    expon_funs,
    unary
} prior_t;

typedef struct Node
{
    prior_t prior;
    type_sign t_val;
    struct Node *next;
} stack_symbol;


void push_sym(stack_symbol **st, prior_t pr, type_sign sign);
stack_symbol *get_sym(stack_symbol **st);
int to_stack(char *str, stack_symbol **st, char **outs);
int digits(char c);
int cut_num(char *str, char *out, int *i);
int is_func(int *i, char *str);
int get_prior(int sign);
int is_oper(char *str, int i);
void check_unar(char *str, int i, int *check_unar);
void move__(stack_symbol **st, int symbol);



int main() {
    stack_symbol *st = NULL;
    char *t = malloc(sizeof(char) * 20);
    to_stack("1.31+3e+12-4+sin", &st, &t);
    printf("out = %s\n", t);
    st = get_sym(&st);
    printf("prior = %d\ttype = %d\n", st->prior, st->t_val);
    free(t);
    return 0;
}

int to_stack(char *str, stack_symbol **st, char **outs) {
    int ret = 0;
    int c_out = 0;
    int check_func = 0;
    int check_oper = 0;
    int i = 0;
    while(str[i] != '\0') {
        if((check_func = is_func(&i, str)) != 0)
            push_sym(st, get_prior(check_func), check_func);
        else if(str[i] == '(')
            push_sym(st, get_prior('('), '(');
        else if(digits(str[i]))
            cut_num(str,*outs + strlen(*outs), &i);
        else if ((check_oper = is_oper(str, i)) != 0) {
            check_unar(str, i, &check_oper);

        }
        else
            i++;
    }
    return ret;
}

void push_sym(stack_symbol **st, prior_t pr, type_sign sign) {
    stack_symbol *st_temp = calloc(1, sizeof(stack_symbol));
    st_temp->prior = pr;
    st_temp->t_val = sign;
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

int digits(char c) {return (c >= '0' && c <= '9');}

int cut_num(char *str, char *out, int *i) {
  int ret = 0;
  while (str[*i] != '\0' && digits(str[*i]) || (str[*i] == '.' && digits(str[*i + 1]) && digits(str[*i - 1]))) {
    out[strlen(out)] = str[*i];
    *i += 1;
  }
  if (str[*i] == 'e' || str[*i] == 'E') {
    out[strlen(out)] = str[*i];
    *i += 1;
    if (str[*i] == '+' || str[*i] == '-') {
        out[strlen(out)] = str[*i];
        *i += 1;
        }
    }
    while (str[*i] != '\0' && digits(str[*i])) {
        out[strlen(out)] = str[*i];
        *i += 1;
        ret++;
    }
    out[strlen(out)] = ' ';
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

int get_prior(int sign) {
    int ret = 0;
    if(sign == unar_pos || sign == unar_neg)
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

int is_oper(char *str, int i) {
    int ret = 0;
    if (str[i] == '+' || str[i] == '-' || str[i] == '/' || str[i] == '*' || str[i] == '%' || str[i] == '^')
        ret = str[i];
    return ret;
}

void check_unar(char *str, int i, int *check_unar) {
    if(str[i - 1] != ')' && !(digits(str[i - 1])) && (str[i] == '-' || str[i] == '+')) {
        if(str[i] == '+')
            *check_unar = '|';
        else if(str[i] == '-')
            *check_unar = '~';
    }
}

void move__(stack_symbol **st, int symbol) {
    
}