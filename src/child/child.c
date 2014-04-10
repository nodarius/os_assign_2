#include <stdio.h>
#include <stdlib.h>
#include "child.h"

int main1(int argc, char **argv)
{
  if (argc < 2) {
    printf("usage: program message_queue_name\n");
    exit(-1);
  }
  printf("%s\n", argv[1]);
  printf("run \n");
  return 0;
}
