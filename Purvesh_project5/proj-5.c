// Purvesh biyani
#include "sem.h"
#define MAX_BUFFER_SIZE 1024
struct TCB_t *runQ = NULL;


int num;
struct semaphore **chop_stick = NULL;

void philo_function(int philoID)
{
    int left_Chop_stick = (philoID - 1) % num;
    int right_chop_stick = (philoID) % num;

    P(chop_stick[left_Chop_stick]);
    yield();

    P(chop_stick[right_chop_stick]);
    yield();

    printf("\n Philosopher %d is eating \n", philoID);
    V(chop_stick[left_Chop_stick]);

    V(chop_stick[right_chop_stick]);

    struct TCB_t *temp = DelQueue(&runQ);
    if(runQ != NULL)
    {
        swapcontext(&(temp->context),&(runQ->context));

    }
}

int main() {
  int num_entries = 3;
  int num_philosophers = 2;
    InitQueue(&runQ);
    scanf("%d,%d", &num_philosophers, &num_entries);
    num = num_philosophers;
   
    chop_stick = (struct semaphore **)malloc(num_philosophers * sizeof(struct semaphore *));
    
    for (int j = 0; j < num_philosophers; ++j) {
      chop_stick[j] = (struct semaphore *)malloc(sizeof(struct semaphore));
      InitSem(chop_stick[j], 1);
    }
    for (int i = 0; i < num_entries; i++) {
        int threadID;
        scanf("%d", &threadID);
        void* function = philo_function;
        startThread(function, threadID);
    }
    run();
    return 0;
}
