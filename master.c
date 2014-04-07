#include <stdio.h>
#include <string.h>


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



int main(int argc, char **argv)
{
  char input[20];
  while(1) {
    fgets(input, 20, stdin);
    if (calc_entered(input)) {
      printf("calc entered\n");
    }
  }
  
  return 0;
}
