#include "../include/ppos.h"
#include "../include/queue.h"
#include <ucontext.h>
#include <stdlib.h>

#define STACKSIZE 64 * 1024

task_t context_main;
task_t *current_context;
int gid = 0;
char *stack;

void ppos_init()
{
  context_main.id = gid;
  context_main.next = NULL;
  context_main.prev = NULL;
  context_main.status = 0;
  getcontext(&context_main.context);

  context_main.context.uc_stack.ss_sp = malloc(STACKSIZE);
  context_main.context.uc_stack.ss_size = STACKSIZE;
  context_main.context.uc_stack.ss_flags = 0;
  context_main.context.uc_link = 0;

  gid++;
  current_context = &context_main;
}

int task_init(task_t *task, void (*start_func)(void *), void *arg)
{
  task->id = gid;
  task->status = 0;
  task->next = NULL;
  task->prev = NULL;
  getcontext(&task->context);

  task->context.uc_stack.ss_sp = malloc(STACKSIZE);
  task->context.uc_stack.ss_size = STACKSIZE;
  task->context.uc_stack.ss_flags = 0;
  task->context.uc_link = 0;

  makecontext(&task->context, (void *)(*start_func), 1, arg);
  gid++;
  return task->id;
}

int task_id()
{
  return current_context->id;
}

void task_exit(int exit_code)
{
  task_switch(&context_main);
}

int task_switch(task_t *task)
{
  task_t *prev_task = current_context;
  current_context = task;

  return swapcontext(&prev_task->context, &task->context);
}
