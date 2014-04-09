#include <stdio.h>
#include <string.h>
#include <ctype.h>

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


int main1(int argc, char **argv)
{
  char input[LINE_LENGTH + 1];
  while(1) {
    fgets(input, LINE_LENGTH, stdin);
    if (calc_entered(input)) {
      printf("calc entered\n");
      break;
    }
    if (valid_input(input)) {
      printf("valid\n");
    }
  }

  return 0;
}
