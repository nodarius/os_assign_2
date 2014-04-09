#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

const int LINE_LENGTH = 50;


/*
 * if input equals "calc" - returns 1
 * else returns 0
 */
int calc_entered(char *input)
{
  if(!strncmp("calc", input, 4)) {
    if (input[4] == '\0' || input[4] == '\n')
      return 1;
  }
  return 0;
}




/*
 * returns 1 if input is valid
 * 0 otherwise
 */
int valid_input(char *input)
{
  if (input == NULL || strlen(input) < 3) return 0;
  int len = strlen(input);
  char ch;
  int current = 0; int i;
  for (i = 0; i < len; i++) { // get rid of starting spaces
    if (isspace(input[i])) continue;
    break;
  }

  if (input[i] == '-') i++; // negative first digit
  current = i;
  for (; i < len; i++) { // get rid of first digit
    ch = input[i];
    if (isdigit(ch)) continue;
    break;
  }
  if (i == current) { // first digit didn't exist
    return 0;
  }

  for(; i < len; i++) { // get rid of spaces after 1st digit
    if (isspace(input[i])) continue;
    break;
  }

  ch = input[i];
  if (ch == '-' || ch == ':' || ch == '/' || ch == '+' || ch == '*') {
    i++;
  } else {
    return 0; // no operator after first digit
  }
  for(; i < len; i++) { // get rid of spaces after operator
    if (isspace(input[i])) continue;
    break;
  }

  current = i;
  for (; i < len; i++) {
    ch = input[i];
    if (isdigit(ch)) continue;
    break;
  }

  if (i == current) return 0; // no second digit
  for(; i < len; i++) { // get rid of trailing whitespace
    if (isspace(input[i])) continue;
    break;
  }

  if (i != len) return 0;


  return 1;
}


struct inputs_t{
  char **data;
  int n_alloc;
  int n_elem;
};


/*
 * adds char *input to inputs_t struct's array (doesn't copy it)
 * increases inputs_t->data size if needed;
 */
int save_input(struct inputs_t *inputs, char *input)
{
  if (inputs->n_elem == inputs->n_alloc) {
    if (inputs->n_alloc == 0) {
      inputs->data = malloc(10 * sizeof(int*));
      inputs->n_alloc = 10;
    } else {
      int new_size = inputs->n_alloc * 2;
      inputs->data = realloc(inputs->data, new_size);
      inputs->n_alloc = new_size;
    }
  }
  inputs->data[inputs->n_elem] = input;
  inputs->n_elem++;
  return 0;
}

int main1(int argc, char **argv)
{
  struct inputs_t inputs;
  inputs.n_elem = 0;
  inputs.n_alloc = 0;

  char input[LINE_LENGTH + 1];
  while(1) {
    fgets(input, LINE_LENGTH, stdin);
    if (calc_entered(input)) {
      printf("calc entered\n");
      break;
    }
    if (valid_input(input)) {
      char *ch = strdup(input);
      save_input(&inputs, ch);
      printf("valid\n");
    }
  }

  return 0;
}
