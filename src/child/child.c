#include <fcntl.h>
#include <sys/stat.h>
#include <mqueue.h>
#include <stdio.h>
#include <stdlib.h>
#include "child.h"



int get_message(char *message, mqd_t mqd)
{
  
}


int main1(int argc, char **argv)
{
  if (argc < 2) {
    printf("usage: program message_queue_name\n");
    exit(-1);
  }

  mqd_t mqd = mq_open(argv[1], O_RDWR);
  if (mqd == (mqd_t) -1) {
    perror("mq_open");
    exit(-1);
  }

  struct mq_attr attr;
  if (mq_getattr(mqd, &attr) == -1) {
    perror("mq_getattr");
    exit(-1);
  }
  

  return 0;
}
