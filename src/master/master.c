#include <stdio.h>
#include <string.h>

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

int valid_input(char *input)
{
  return 0;
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
