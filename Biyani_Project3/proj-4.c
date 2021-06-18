
// Purvesh biyan
#include "sem.h"
#define MAX_BUFFER_SIZE 1024
#define NUM_THREADS 3
struct pair_int_int {
    int item;
    int pid;
} buffer[MAX_BUFFER_SIZE];
struct TCB_t *runQ = NULL;
struct semaphore *rmutex = NULL, *wmutex = NULL;
int rc = 0; 
int rwc = 0; 
int wwc = 0; 
int wc = 0; 
int resource_val = 0;
void reader(int);
void writer(int);

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

    rmutex = (struct semaphore *)malloc(sizeof(struct semaphore));
    InitSem(rmutex,0);

    wmutex = (struct semaphore *)malloc(sizeof(struct semaphore));
    InitSem(wmutex,0);

    run(); //starts the first thread
    return 0;
}

void reader(int readerID)
{
    if (wwc > 0 || wc > 0)
    {
        rwc++;
        P(rmutex);
        rwc--;
    }
    rc++;
    printf("\n This is the %d th reader reading value i = %d for the first time \n", readerID, resource_val);
    yield();
    printf("\n This is the %d th reader reading value i = %d for the second time \n", readerID, resource_val);

    rc--;
    if (rc == 0 && wwc > 0)
    {
        V(wmutex);
    }

        struct TCB_t *temp= DelQueue(&runQ);
    if(temp!=NULL && runQ!=NULL)
        swapcontext(&(temp->context),&(runQ->context));
}

void writer(int writerID)
{
    if (rc > 0 || wc > 0 || rwc > 0 || wwc > 0)
    {

        wwc++;
        P(wmutex);
        wwc--;
    }
    wc++;

    resource_val++;
    printf("\n This is the %d th writer writing value i = %d \n", -writerID, resource_val );
    yield();
    printf("\n This is the %d th writer verifying value i = %d \n", -writerID, resource_val );

    wc--;
    if (rwc > 0)
    {
        for (int j = 0; j < rwc; j++) V(rmutex);
    } else if (wwc > 0) {
        V(wmutex);
    }

    struct TCB_t *temp= DelQueue(&runQ);
    if(temp!=NULL && runQ!=NULL)
        swapcontext(&(temp->context),&(runQ->context));
}