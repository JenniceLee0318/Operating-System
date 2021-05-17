#ifndef _PCB_H_
#define _PCB_H_
#include <stdbool.h>
typedef struct PCB_S PCB;
struct PCB_S{
 int pid;         //process id
 int priority;    //process priority
 char proState;   //process state
 char *proc_message;   //================================================================
 int msgTo;
 PCB *next;
 PCB *prev;
};

typedef struct Queue_S Queue;
struct Queue_S{
 PCB *head;
 PCB *tail;
 PCB *run;
 PCB *init;
 int priority;
 int processCnt; // number of process in the Queue
 int timeQuantum; // Time Quantum of this ready Queue
};


typedef struct Semaphore_s Semaphore;
struct Semaphore_s{
int value;
Queue *sem_list;
};

//declaration static
static Queue QueueArray[10];
static Semaphore SemaphoreArray[5];
static int TotalQueueCnt = 0;
static int MaxId = 0; 
static Queue *readyQueue[5];  //array of queue 0 ~ 2 priority ===========================
static Semaphore *proList[5];


//declaration function
Queue* createQueue(int priority);
int createProcess(int priority, Queue** queue);
void TotalInfo();
void Proc_info(int pid);
int  Kill(int pid);
int Exit();
int Fork();
int Quantum();
//void init();
int process_run();
int Send(int pid, char* Msg); 
int checkProcess(int pid);
int Receive();
int Reply(int pid, char* msg);
int New_semaphore(int id, int value);
void Semaphore_List();
int Semaphore_P(int id);
int Semaphore_V(int id);
//ProState: R: Ready A: Run(Active) B: Blocked
#endif