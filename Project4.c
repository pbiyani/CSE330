#include "sem.h"

#define MAX_BUFFER_SIZE 1024
#define NUM_THREADS 3


struct TCB_t *runQ = NULL;

struct semaphore *rmutex = NULL, *wmutex = NULL,  *mutex = NULL;

int rc = 0; //reader count
int rwc = 0; //reader waiting count
int wwc = 0; //writer waiting count
int wc = 0; //write count
int resource_val = 0;

/********************
 * Function Prototypes
 ********************/
void reader(int);
void writer(int);
void readerEntry();
void readerExit();
void writerEntry();
void writerExit();

int main() {
	int numReader = 0, numWriter = 0;
	scanf("%d,%d", &numReader, &numWriter);
	struct TCB_t *threads[numReader + numWriter];

	InitQueue(&runQ);
	int i;
	for (i = 0; i < numReader + numWriter; i++) {
		int pid;
		scanf("%d", &pid);
		if (pid > 0)
			startThread(threads[i], reader, numReader, pid);
		else
			startThread(threads[i], writer, numWriter, pid);
	}


	mutex = (struct semaphore *)malloc(sizeof(struct semaphore));
	//InitQueue(mutex->queue);
	InitSem(mutex, 1);

	rmutex = (struct semaphore *)malloc(sizeof(struct semaphore));
	//InitQueue(rsem->queue);
	InitSem(rmutex,1);

	wmutex = (struct semaphore *)malloc(sizeof(struct semaphore));
	//InitQueue(wsem->queue);
	InitSem(wmutex,1);

	run(); //starts the first thread
	return 0;
}

void reader(int pid)
{
	int turn=0;
	char *first="first";
	char *second="second";
	while(++turn<3){
		readerEntry();
		printf("\nThis is the %d th reader reading value i = %d for the %s time" , pid, resource_val, turn == 1 ? first : second);
		readerExit();
		if (turn<2)
			yield();
	}
	// exit reader and give CPU to next thread in queue
	struct TCB_t *temp= DelQueue(&runQ);
	if(temp!=NULL && runQ!=NULL)
		swapcontext(&(temp->context),&(runQ->context));
}


void writer(int pid)
{
	writerEntry();
	printf("\nThis is the %d th writer writing value i = %d", -pid, ++resource_val);
	writerExit();
	yield();
	readerEntry();
	printf("\nThis is the %d th writer verifying value i = %d", -pid, resource_val);
	readerExit();

	// exit writer and give CPU to next thread in queue
	struct TCB_t *temp= DelQueue(&runQ);
	if(temp!=NULL && runQ!=NULL)
		swapcontext(&(temp->context),&(runQ->context));

}

void readerEntry(){
	 P(mutex);
     if(wwc>0 || wc>0){
	 	rwc++;
	 	V(mutex);
		P(rmutex);
	// 	P(mutex);
		rwc--;
	}
	if(rwc > 0){
		V(rmutex);
	}
	 else{
	 	V(mutex);
	 }
	 

}

void readerExit(){
	P(mutex);
	rc--;
	if(rc==0 && wwc>0){
		V(wmutex);
	} 
	else{
		V(mutex);
	}	
}

void writerEntry(){
	P(mutex);
	//|| rwc>0 || wwc>0
    if(rc>0 || wc>0 ) {
		wwc++;
		V(mutex);
		P(wmutex);
		//P(mutex);
		wwc--;
	}
	wc++;
	V(mutex);
}

void writerExit(){
	P(mutex);
	wc--;
	if(rwc>0) {
		//for (int i=0;i<rwc;i++) 
		V(rmutex);
	}
	else if (wwc>0){
		V(wmutex);
	} 
}