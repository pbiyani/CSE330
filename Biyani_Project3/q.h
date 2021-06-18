//Purvesh Biyan
#ifndef Q_H
#define Q_H

#include<stdio.h>
#include<stdlib.h>

#include "tcb.h" 

struct TCB_t *NewItem()
{
	struct TCB_t *item;
	item=(struct TCB_t*) malloc(sizeof(struct TCB_t));
	return item;
}
// Initializing the Queue
void InitQueue(struct  TCB_t **headA)
{
	*headA=(struct TCB_t*) malloc(sizeof(struct TCB_t));	
	*headA=NULL;	
}

// Adding elements to queue
void AddQueue(struct TCB_t **headA, struct TCB_t *item)
{
		
	if(*headA==NULL)
		{
		*headA=item;
		item->prev=item;
		item->next=item;
		}

	else   
		{
		struct TCB_t *tempNode1;
		struct TCB_t *tempNode2;
		tempNode1=*headA;
		tempNode2=tempNode1->prev;

		item->next=tempNode1;
		item->prev=tempNode2;

		tempNode1->prev=item;
		tempNode2->next=item;
		}
		
}	


struct TCB_t *DelQueue(struct TCB_t **headA)
{
		
	if(*headA==NULL)
		{
		
		return NULL;
		}

	else   
	{
		
		struct TCB_t *tempNode1;
		tempNode1=*headA;

		if (tempNode1->next==tempNode1)
			{
			*headA=NULL;
			}
		else
			{
			struct TCB_t *tempNode2;
			struct TCB_t *tempNode3;

			tempNode2=tempNode1->next;
			tempNode3=tempNode1->prev;
			tempNode2->prev=tempNode3;
			tempNode3->next=tempNode2;
		
			*headA=tempNode2;
			}

		//clear old node

		tempNode1->next=NULL;
		tempNode1->prev=NULL;
	return tempNode1;
	}	
}
void RotateQ(struct TCB_t **headA)
{
	if(*headA==NULL)
		{
		}

	else  
		{
		struct TCB_t *firstNode;
		
		firstNode=*headA;
		if(firstNode->next==firstNode)
				{
				}
		else
				{
				*headA=firstNode->next;
				}
		}
}

void printQueue(struct TCB_t **headA)
{
	if(*headA==NULL)
		{
		printf("Queue is Empty .. \n");
		}

	else   /// queue not empty.
		{
		struct TCB_t *firstNode;
		struct TCB_t *runNode;
		firstNode=*headA;
		runNode=firstNode;
			if(firstNode->next!=NULL)
				{
				  do{
					
					printf("%d ", runNode->pid);
					runNode=runNode->next;
					
				  }while(runNode!=firstNode);
				}			
		}
	
}

#endif
