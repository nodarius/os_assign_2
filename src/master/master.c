#include <semaphore.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <mqueue.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>

const int LINE_LENGTH = 50;
const char *QUEUE_NAME = "/M_QUEUE12";
const char *SEMAPHORE_NAME = "TECH_N9NE_1";
const int N_CHILD = 2;

/*
 * if input equals "calc" - returns 1
 * else returns 0
 */
int calc_entered(const char *input)
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
int valid_input(const char *input)
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



int create_mqueue()
{
  int flags;
  mqd_t mqd;
  struct mq_attr attr;

  attr.mq_maxmsg = 10;
  attr.mq_msgsize = 50;
  flags = O_RDWR | O_CREAT;

  umask(0);
  mqd = mq_open(QUEUE_NAME, flags, 0666, &attr);

  if (mqd == (mqd_t) -1) {
    printf("error number: %d", errno);
    perror("mq_open");
    exit(-1);
  }

  return mqd;
}

/*
 * adds all char*-s from inputs->data to mqd
 * and frees inputs->data;
 */
int add_to_queue(struct inputs_t *inputs, mqd_t mqd)
{
  int i;
  int status;
  for (i = 0; i < inputs->n_elem; i++) {
    status = mq_send(mqd, inputs->data[i], strlen(inputs->data[i]) + 1, 0);
    if (status == -1) {
      perror("could not send");
      exit(-1);
    }
    free(inputs->data[i]);
  }
  inputs->n_elem = 0;

  return 0;
}

/*
 * creates new child process and returns it's ID to parent
 * 
 */
pid_t create_child()
{
  pid_t pid = fork();
  if (pid == (pid_t) -1) {
    perror("fork");
    exit(-1);
  }
  if (pid == 0) { // child
    char *arg1 = strdup(QUEUE_NAME);
    char *arg2 = strdup(SEMAPHORE_NAME);
    char *new_arg[] = {"./child", arg1, arg2};
    char *new_env[] = {NULL};
    execve("child", new_arg, new_env);
    perror("execve"); // execve doesn't return on success
    exit(-1);
  } else { // parent
    return pid; // child id;
  }
}


int main1(int argc, char **argv)
{
  mqd_t mqd = create_mqueue();
  sem_t *sem = sem_open(SEMAPHORE_NAME, O_CREAT, S_IRUSR | S_IWUSR, 1);
  if (sem == SEM_FAILED) {
    perror("sem_open");
    exit(-1);
  }

  pid_t child1 = create_child();
  pid_t child2 = create_child();


  struct inputs_t inputs;
  inputs.n_elem = 0;
  inputs.n_alloc = 0;

  char input[LINE_LENGTH + 1];
  while(1) {
    fgets(input, LINE_LENGTH, stdin);
    if (calc_entered(input)) {

      sem_wait(sem);

      printf("Adding to message queue..");
      add_to_queue(&inputs, mqd);
      printf(" Done.\n");

      sem_post(sem);

      continue;
    }
    if (valid_input(input)) {
      char *ch = strdup(input);
      save_input(&inputs, ch);
    } else {
      printf("Invalid input.\n");
    }
  }


  sem_close(sem);
  sem_unlink(SEMAPHORE_NAME);
  mq_close(mqd);
  mq_unlink(QUEUE_NAME);
  return 0;
}
