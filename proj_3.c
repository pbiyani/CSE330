// Purvesh biyani
// 1215598839
// Project 2 

#include "sem.h"
#define MAX_BUFFER_SIZE 1024
#define NUM_THREADS 3

struct TCB_t *runQ = NULL;
struct semaphore *full = NULL, *empty = NULL;

int in = 0, out = 0;

struct pair_int_int {
    int item;
    int pid;
} buffer[MAX_BUFFER_SIZE];
// Function created for the producer
void producer(int buffer_size, int pid, int maxLoops)
{
	int i=0;
	int items=0;
	while (i++<maxLoops) {
		char str[80];
		sprintf(str, "\n Producer %d is waiting \n", pid);
		P(full, str);
		struct pair_int_int temp = {.item= ++items, .pid=pid};
		buffer[in] = temp;
		printf("\n Producer %d is producing item number %d \n", pid, buffer[in]);
		//sleep(1);
		in = (in + 1) % buffer_size;
		V(empty);
	}
	struct TCB_t *temp= DelQueue(&runQ);
	if(temp!=NULL && runQ!=NULL)
		swapcontext(&(temp->context),&(runQ->context));
}
// Function cvreated for the consumer
void consumer(int buffer_size, int pid, int maxLoops)
{
	int i=0;
	while(i++<maxLoops) {
		char str[80];
		sprintf(str, "\n Consumer %d is waiting \n", -pid);
		P(empty, str);
		printf("\n Consumer %d is consuming item generated by Producer %d \n", -pid, buffer[out].pid);
		//sleep(1);
		out = (out + 1) % buffer_size;
		V(full);
	}
	struct TCB_t *temp = DelQueue(&runQ);
	if(temp!=NULL && runQ!=NULL)
		swapcontext(&(temp->context),&(runQ->context));

}


int main()
{
	int B,P,C,N;
	scanf("%d,%d,%d,%d", &B,&P, &C,&N);
	
	struct TCB_t *threads[P+C];

	full = (struct semaphore *)malloc(sizeof(struct semaphore));
	empty = (struct semaphore *)malloc(sizeof(struct semaphore));

	InitSem(empty, 0);
	InitSem(full, B);

	InitQueue(&runQ);

	int i;
	for (i = 0; i < P+C; i++) {
		int pid;
		scanf("%d", &pid);
		if (pid > 0)
			startThread(threads[i], producer, B, pid, N);
		else
			startThread(threads[i], consumer, B, pid, N);
	}
	run();

	return 0;
}