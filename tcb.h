// Purvesh Biyani
#ifndef TCB_H
#define TCB_H

#include <ucontext.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

struct TCB_t {
     struct TCB_t *next;
     struct TCB_t *prev;
     ucontext_t context;
     int pid;
};

void init_TCB(struct TCB_t *tcb, void (*function)(int), int pid, void *stack, int stack_size)
{
    memset(tcb, '\0', sizeof(struct TCB_t));     
    getcontext(&tcb->context);              
    tcb->context.uc_stack.ss_sp = stack;
    tcb->context.uc_stack.ss_size = (size_t) stack_size;
    tcb->pid=pid;
    makecontext(&tcb->context, function, 1, pid);
}

#endif
