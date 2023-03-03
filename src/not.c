#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

typedef enum {
    mod = 37,
    openB = 40,
    closeB,
    mul = 42,
    sum = 43,
    del = 45,
    divi = 47,
    pow = 94,
    coss = 99,
    sinn = 115,
    tann = 116,
    acoss = 67,
    asinn = 83,
    atann = 84,
    sqrtt = 113,
    lnn = 110,
    logg = 103,
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

typedef struct st {
    int symbol;
    struct st *next;
} stack;

int polish(char *str, char **out);
void push(stack **st, int symbol);
int del_peak(stack **st);
int cut_num(char *str, char *out, int *i);
int digits(char c);
int is_func(int *i, char *str);
void move_to_out(stack **st, char **out);
int is_empty(stack *st);
int is_oper(char c);
int is_unary(char *str, int *i);
int get_prior(int sign);


int main() {
    char *str = NULL;
    str = calloc(40, sizeof(char));
    polish("(1+2)*(3+4)-1", &str);
    printf("%s\n", str);
    free(str);
    return 0;
}




int polish(char *str, char **out) {
    int ret = 0;
    int i = 0;
    int check_func = 0;
    int check_unar = 0;
    //int stop_brace = 0;
    stack *st = NULL;
    //char *temp = NULL;
    while(str[i] != '\0' && ret == 0) {
        if(digits(str[i])) cut_num(str,*out + strlen(*out), &i);
        else if((check_func = is_func(&i, str)) != 0)  push(&st, check_func);
        else if(str[i] == '(') {
            push(&st, str[i]);
            i++;
        } else if ((check_unar = is_unary(str, &i)) != 0) push(&st, check_unar);
        else if(is_oper(str[i]) != 0) {
            if(is_empty(st)) push(&st, str[i]);
            else { while(get_prior(st->symbol) > get_prior(str[i])) {
                move_to_out(&st, out);} }
            push(&st, str[i]);
            
            i++;
        } else if(str[i] == ')') {
            while(!is_empty(st)) {
                if(st->symbol != '(')
                    move_to_out(&st, out);
                else
                    del_peak(&st);
            }
            i++;
        }
        else {
            i++;
        }
    }
    printf("\n%s\n", *out);
    while(st != NULL)
        move_to_out(&st, out);
    return ret;
}

int is_unary(char *str, int *i) {
    int ret = 0;
    if(str[*i - 1] != ')' && digits(str[*i + 1]) && !digits(str[*i - 1])) {
        if(str[*i] == '-')
            ret = '~';
        else if(str[*i] == '+')
            ret = '|';
    }
    if(ret != 0)
        *i +=1;
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
    else if(sign >= 95 && sign <= 103)
        ret = expon_funs;
    return ret;
}


void move_to_out(stack **st, char **out) {
    char *temp = calloc(3 ,sizeof(char));
    if(temp != NULL) {
        temp[0] = del_peak(st);
        temp[1] = ' ';
        strcpy(*out + strlen(*out), temp);
    }
    free(temp);
}



int is_empty(stack *st) {return(st == NULL);}


void push(stack **st, int symbol) {
    stack *temp = NULL;
    if((temp = calloc(1, sizeof(stack))) != NULL) {
        temp->symbol = symbol;
        temp->next = *st;
        *st = temp;
    }
}

int del_peak(stack **st) {
    int ret = -1;
    stack *temp = NULL;
    if(*st != NULL) {
        temp = *st;
        ret = temp->symbol;
        *st = (*st)->next;
        free(temp);
    }
    return ret;
}

int is_oper(char c) {
    int ret = 0;
    if (c == '+' || c == '-' || c == '/' || c == '*' || c == '%' || c == '^')
        ret = c;
    return ret;
}

int cut_num(char *str, char *out, int *i) {
  int ret = 0;
  while ((str[*i] != '\0' && digits(str[*i])) || (str[*i] == '.' && digits(str[*i + 1]) && digits(str[*i - 1]))) {
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

int digits(char c) {return (c >= '0' && c <= '9');}

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
        else if(strncmp("log", str + *i, 3) == 0)
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