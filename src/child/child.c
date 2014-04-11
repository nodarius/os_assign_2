#define _GNU_SOURCE
#include <ctype.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <signal.h>
#include <unistd.h>
#include <semaphore.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <mqueue.h>
#include <stdio.h>
#include <stdlib.h>
#include "child.h"

#define NOTIFY_SIG SIGUSR1

char get_operator(char *input)
{
  int len = strlen(input);
  int i;
  for (i = 0; i < len; i++) {
    if (input[i] == '-') return '-';
    if (input[i] == '+') return '+';
    if (input[i] == '*') return '*';
    if (input[i] == '/') return '/';
    if (input[i] == ':') return ':';
  }
  printf("INVALID INPUT\n"); // parent must have checked it;
  exit(-1);
  return '\0';
}

struct result_t{
  int first;
  int second;
  char operator;
  double result;
};

// terrible code
struct result_t calculate(char *input)
{
  char operator = get_operator(input);
  int is_negative = 0;
  int len = strlen(input);
  int i;
  for (i = 0; i < len; i++) {
    if (isspace(input[i])) continue;
    break;
  }
  char *ptr = &input[i];
  if (*ptr == '-') { // first number is negative
    is_negative = 1;
    ptr++;
  }

  int f_len = 0;
  while(isdigit(ptr[f_len++]));
  f_len--;
  ptr[f_len] = '\0';

  int f_num = atoi(ptr);
  if (is_negative) f_num *= -1;

  ptr += f_len;
  i = 0;
  while(!isdigit(ptr[i++]));
  i--;
  ptr += i;
  int s_len = 0;
  while(isdigit(ptr[s_len++]));
  ptr[s_len] = '\0';

  int s_num = atoi(ptr);
  
  printf("%d %c %d\n", f_num, operator, s_num);

  struct result_t result;
  result.first = f_num;
  result.second = s_num;
  if (operator == '-') {
    result.result = f_num - s_num;
  }
  if (operator == '+') {
    result.result = f_num + s_num;
  }
  if (operator == '*') {
    result.result = f_num * s_num;
  }
  if (operator == '/' || operator == ':') {
    result.result = (double) f_num / s_num;
  }
  result.operator = operator;
  
  return result;
}

void handler() {}

int main1(int argc, char **argv)
{
  if (argc < 3) {
    printf("usage: program message_queue_name semaphore_name\n");
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

  sem_t *sem = sem_open(argv[2], 0);
  if (sem == SEM_FAILED) {
    perror("sem_open");
    exit(-1);
  }

  char buffer[attr.mq_msgsize];

  while(1) {
    int status = mq_receive(mqd, buffer, attr.mq_msgsize, 0);
    if (status == -1) {
      perror("mq_receive");
      exit(-1);
    }
    buffer[status + 1] = '\0';
    sem_wait(sem);
    struct result_t result = calculate(buffer);
    printf("%d: %d %c %d = %f\n", getpid(), result.first, 
	   result.operator, result.second, result.result);
    sem_post(sem);
  }

  return 0;
}
