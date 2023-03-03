#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

typedef enum {
  mod = 37,
  openB = 40,
  closeB,
  mul = 42,
  sum = 43,
  del = 45,
  divi = 47,
  powr = 94,
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

typedef struct num {
  double num;
  struct num *next;
} number;

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

//=====//

int calc(char *str, double *res);
double del_peak_num(number **st);
void push_num(number **st, double num);
double operation(double right, double left, int oper) ;
int is_short_fun (char c);
double funstion(double peak, int oper);


int main() {
  char *str = NULL;
  str = calloc(40, sizeof(char));
  double ret = 0;
  if(polish("log(10)--cos(10)", &str) == 0) {
      if(calc(str, &ret) == 0)
        printf("res = %lf\n", ret);
  } 
  printf("%s\n", str);
  free(str);
  return 0;
}

int calc(char *str, double *res) {
    int ret = 0;
    number *num = NULL;
    int i = 0;
    char *temp = NULL;
    double num_temp;
    while(str[i] != '\0') {
        num_temp = 0;
        if(isspace(str[i])) i++;
        if(digits(str[i])) {
            temp = calloc(20, sizeof(char));
            cut_num(str, temp, &i);
            push_num(&num, atof(temp));
            free(temp);
        } else if(str[i] == '~') {
            num_temp = del_peak_num(&num) * -1;
            push_num(&num, num_temp);
            i++;
        } else if(is_oper(str[i]) != 0) {
            if(num != NULL && num->next != NULL) {
                num_temp = operation(del_peak_num(&num),del_peak_num(&num), str[i]);
                push_num(&num, num_temp);
            }
            i++;
        } else if(is_short_fun(str[i])) {
            num_temp = funstion(del_peak_num(&num), str[i]);
            push_num(&num, num_temp);
            i++;
        }
        else  i++;
    }
    
    if(num != NULL) {
        if(num != NULL && num->next == NULL)
            *res = del_peak_num(&num);
        else
            ret = -1;
    } else 
        ret = -1;
    return ret;
}

double operation(double left, double right, int oper) {
    double num = 0;
    if(oper == '+')
        num = left + right;
    else if(oper == '-')
        num = left - right;
    else if(oper == '*')
        num = left * right;
    else if(oper == '/')
        num = left / right;
    else if(oper == '%')
        num = fmodl(left, right);
    else if(oper == '^')
        num = powl(left, right);
    printf("after_oper\t%lf %c %lf = %lf\n", left, oper, right, num);
    return num;
}

int is_short_fun (char c) {return (c == 'c' || c == 's' || c == 't' || c == 'C' || c == 'S' || c == 'T' || c == 'q' || c == 'n' || c == 'g');}

double funstion(double peak, int oper) {
    double num = 0;
    if(oper == 'c')
        num = cos(peak);
    else if(oper == 's')
        num = sin(peak);
    else if(oper == 't')
        num = tan(peak);
    else if(oper == 'C')
        num = acos(peak);
    else if(oper == 'S')
        num = asin(peak);
    else if(oper == 'T')
        num = atan(peak);
    else if(oper == 'q')
        num = sqrt(peak);
    else if(oper == 'n')
        num = log(peak);
    else if(oper == 'g')
        num = log10(peak);
    printf("after_oper\t%c(%lf) = %lf\n", oper, peak, num);
    return num;
}

void push_num(number **st, double num) {
  number *temp = NULL;
  if ((temp = calloc(1, sizeof(number))) != NULL) {
    temp->num = num;
    temp->next = *st;
    *st = temp;
  }
}

double del_peak_num(number **st) {
  double ret = -1;
  number *temp = NULL;
  if (*st != NULL) {
    temp = *st;
    ret = temp->num;
    *st = (*st)->next;
    free(temp);
  }
  return ret;
}



//  ====    =====   ///

int polish(char *str, char **out) {
  int ret = 0;
  int i = 0;
  int check_func = 0;
  int check_unar = 0;
  stack *st = NULL;
  while (str[i] != '\0' && ret == 0) {
    if (digits(str[i]))
      cut_num(str, *out + strlen(*out), &i);
    else if ((check_func = is_func(&i, str)) != 0)
      push(&st, check_func);
    else if (str[i] == '(') {
      push(&st, str[i]);
      i++;
    } else if ((check_unar = is_unary(str, &i)) != 0)
      push(&st, check_unar);
    else if (is_oper(str[i]) != 0) {
      while (st != NULL && get_prior(st->symbol) > get_prior(str[i]))
        move_to_out(&st, out);
      push(&st, str[i]);
      i++;
    } else if (str[i] == ')') {
      while (st != NULL && st->symbol != '(') move_to_out(&st, out);
      if (st != NULL) del_peak(&st);
      else ret = -1;
      if(is_short_fun(st->symbol))
        move_to_out(&st, out);
      i++;
    } else
      i++;
  }
  if(st != NULL && st->symbol == '(') ret = -1;
  else {while (st != NULL) move_to_out(&st, out);}
  return ret;
}

int is_unary(char *str, int *i) {
  int ret = 0;
  if (str[*i - 1] != ')' && digits(str[*i + 1]) && !digits(str[*i - 1])) {
    if (str[*i] == '-')
      ret = '~';
    else if (str[*i] == '+')
      ret = '|';
  }
  if (ret != 0) *i += 1;
  return ret;
}

int get_prior(int sign) {
  int ret = 0;
  if (sign == unar_pos || sign == unar_neg)
    ret = unary;
  else if (sign == openB)
    ret = open_b;
  else if (sign == closeB)
    ret = close_b;
  else if (sign == sum || sign == del)
    ret = plus_minus;
  else if (sign == divi || sign == mul || sign == mod)
    ret = div_mul_mod;
  else if (sign >= 94 && sign <= 103)
    ret = expon_funs;
  return ret;
}

void move_to_out(stack **st, char **out) {
  char *temp = calloc(3, sizeof(char));
  if (temp != NULL) {
    temp[0] = del_peak(st);
    temp[1] = ' ';
    strcpy(*out + strlen(*out), temp);
  }
  free(temp);
}

int is_empty(stack *st) { return (st == NULL); }

void push(stack **st, int symbol) {
  stack *temp = NULL;
  if ((temp = calloc(1, sizeof(stack))) != NULL) {
    temp->symbol = symbol;
    temp->next = *st;
    *st = temp;
  }
}

int del_peak(stack **st) {
  int ret = -1;
  stack *temp = NULL;
  if (*st != NULL) {
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
  while ((str[*i] != '\0' && digits(str[*i])) ||
         (str[*i] == '.' && digits(str[*i + 1]) && digits(str[*i - 1]))) {
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

int digits(char c) { return (c >= '0' && c <= '9'); }

int is_func(int *i, char *str) {
  int ret = 0;
  if (str[*i] >= 'a' && str[*i] <= 'z') {
    if (strncmp("cos", str + *i, 3) == 0)
      ret = coss;
    else if (strncmp("sin", str + *i, 3) == 0)
      ret = sinn;
    else if (strncmp("tan", str + *i, 3) == 0)
      ret = tann;
    else if (strncmp("acos", str + *i, 4) == 0)
      ret = acoss;
    else if (strncmp("asin", str + *i, 4) == 0)
      ret = asinn;
    else if (strncmp("atan", str + *i, 4) == 0)
      ret = atann;
    else if (strncmp("sqrt", str + *i, 4) == 0)
      ret = sqrtt;
    else if (strncmp("ln", str + *i, 4) == 0)
      ret = lnn;
    else if (strncmp("log", str + *i, 3) == 0)
      ret = logg;
  }
  if (ret == tann || ret == sinn || ret == coss || ret == logg)
    *i += 3;
  else if (ret == acoss || ret == asinn || ret == atann || ret == sqrtt)
    *i += 4;
  else if (ret == lnn)
    *i += 2;
  return ret;
}