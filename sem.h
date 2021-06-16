// Purvesh Biysni

#ifndef SEM_H
#define SEM_H

#include "threads.h"
#include <stdio.h>
struct semaphore
{
	int sem;
	struct TCB_t *q;
};

void InitSem(struct semaphore *semA, int value)
{
	semA->sem=value;
	InitQueue(&(semA->q));	
}

void P(struct semaphore *semA) // similar to run
{
	struct TCB_t *temp;
MYLABEL:semA->sem = semA->sem- 1;
   	if(semA->sem < 0)
	{	
		semA->sem=0;
		struct TCB_t *temp;	
		temp=DelQueue(&runQ); // Deletes a thread
		AddQueue(&(semA->q),temp);

		if (runQ == NULL)
			exit(0);
		swapcontext(&(temp->context),&(runQ->context));
		goto MYLABEL;	
	}
}

void V(struct semaphore *semA)
{
	struct TCB_t *temp;		
	(semA->sem)++;	
	temp=DelQueue(&(semA->q)); // Deletes a thread
	if(temp!=NULL)
	{
		AddQueue(&runQ,temp);
	}
	yield();
}

#endif
