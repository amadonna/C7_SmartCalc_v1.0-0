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

typedef struct st {
    int symbol;
    struct st *next;
} stack;

int polish(char *str, char **out);
stack *push(stack **st, int symbol);
int del_peak(stack **st);
int cut_num(char *str, char *out, int *i);
int digits(char c);
int is_func(int *i, char *str);
void move_to_out(stack **st, char **out);
int is_empty(stack *st);


int main() {
    char *str = NULL;
    str = realloc(str, sizeof(char));
    polish("cos+1+1+1*2+sin", &str);
    printf("%s\n", str);
    free(str);
    return 0;
}


int polish(char *str, char **out) {
    int ret = 0;
    int i = 0;
    int check_func = 0;
    stack *st = NULL;
    //char *temp = NULL;
    while(str[i] != '\0' && ret == 0) {
        if(digits(str[i]))
            cut_num(str,*out + strlen(*out), &i);
        else if((check_func = is_func(&i, str)) != 0) {
            push(&st, check_func);
        }
        else if(str[i] == '(') {
            push(&st, str[i]);
            i++;
        }
        else {
            push(&st, str[i]);
            i++;
        }
    }
    
    while(st != NULL){
        move_to_out(&st, out);
        // temp = calloc(3 ,sizeof(char));
        // if(temp != NULL) {
        //     temp[0] = del_peak(&st);
        //     temp[1] = ' ';
        //     strcpy(*out + strlen(*out), temp);
        // }
    }
    
    return ret;
}

void move_to_out(stack **st, char **out) {
    char *temp = calloc(3 ,sizeof(char));
    if(temp != NULL) {
        
        temp[0] = del_peak(st);
        temp[1] = ' ';
        printf("%c\n", temp[0]);
        strcpy(*out + strlen(*out), temp);
    }
    free(temp);
}

int is_empty(stack *st) {return(st == NULL);}


stack *push(stack **st, int symbol) {
    stack *temp = NULL;
    if((temp = calloc(1, sizeof(stack))) != NULL) {
        temp->symbol = symbol;
        temp->next = *st;
        *st = temp;
    }
    return temp;
}

int del_peak(stack **st) {
    int ret = -1;
    stack *temp = NULL;
    if(*st != NULL) {
        temp = *st;
        ret = temp->symbol;
        *st = temp->next;
        free(temp);
    }
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
        printf("%c\n", ret);
    }
    if(ret == tann || ret == sinn || ret == coss || ret == logg)
        *i += 3;
    else if(ret == acoss || ret == asinn || ret == atann || ret == sqrtt)
        *i += 4;
    else if(ret == lnn)
        *i += 2;
    return ret;
}