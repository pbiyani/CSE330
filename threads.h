// Purvesh Biyani
#ifndef THREADS_H
#define THREADS_H

#include "q.h"
#include<stdio.h>

extern struct TCB_t *runQ;
// Thread is being started
void startThread(struct TCB_t *thread, void (*function)(int), int buffer_size, int pid)
{
	void *stack = (void *)malloc(8192);
	thread = (struct TCB_t *)malloc(sizeof(struct TCB_t));
	init_TCB(thread, function, pid, stack, 8192);

	AddQueue(&runQ, thread);
}
// Run function
void run()
{
	ucontext_t parent;
	getcontext(&parent);
	swapcontext(&parent, &(runQ->context));
}
// yield function
void yield()
{
	struct TCB_t *temp = runQ;
	if(temp == NULL)
		exit(0);
	RotateQ(&runQ);
	swapcontext(&(temp->context), &(runQ->context));
}

#endif
