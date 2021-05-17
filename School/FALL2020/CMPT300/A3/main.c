#include <stdbool.h>
#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include "pcb.h"
int main(){
 bool process = true;
 //create readyQueue (priority: 0,1,2)
 for(int i=0; i < 5; i ++){ //======================================================================
  if(readyQueue[i] == NULL){
      readyQueue[i] = createQueue(i);
   }
 }

//init process
createProcess(2,readyQueue);

printf("Input Command Letter \n");
//Command Control
//ket INPUT CHECK !!!! 
 while(process){
  char userInput = getchar();
  if(userInput == 'C'){ // C: Create a Process   
     
     int inputPriority;
     printf("please input priority of your process between 0 to 2: \n");
     scanf("%d",&inputPriority);
     if(!((int)inputPriority > -1 && (int)inputPriority <3)){
         printf("please enter intefer between 0 to 2 \n");
      }else{
         createProcess(inputPriority,readyQueue);
         printf("Process created \n");
      }  
 
   }else if(userInput == 'T'){ //T: Display all process queues
    
      TotalInfo();
 
   }else if(userInput == 'I'){ //I: Display process info
      int inputPid = 0;
      printf("please input pid of the process you want to know \n");
      scanf("%d",&inputPid);
      Proc_info(inputPid);

   }else if(userInput == 'K'){ //K: Kill the process
 
     int inputPid = 0;
     int result = 0;
     printf("please input pid of the process you want to kill \n");
     scanf(" %d", &inputPid);
     result = Kill(inputPid);
  
     if(result == 1){
        printf("success to kill process pid %d \n", inputPid);
     }else if(result == -1){
        printf("fail to kill process pid %d \n", inputPid);
     }

     TotalInfo();

  }else if(userInput == 'E'){ //E: Exit the process
     int result = 0;
     result = Exit();
  
     if(result == 1){
        printf("success to exit running process \n");
     }else if(result == -1){
        printf("fail to exit running process \n");
     }
     TotalInfo();

  }else if(userInput == 'Q'){ //Q: Quantum the process
     int outputpid = 0;
     outputpid = Quantum();
     Proc_info(outputpid);

  }else if(userInput == 'F'){ //F: Fork the process
     int result = 0;
     result = Fork();
  
     if(result == 1){
        printf("success to fork running process \n");
     }else if(result == -1){
        printf("fail to fork running process \n");
     }
     TotalInfo();

  }else if(userInput == 'S'){ //S: Send  =========================================================
     int Pid = 0;
     char *Msg = malloc(sizeof(char)*40);
     
     printf("please input pid and Msg  you want to send message: \n");
     scanf("%d %[^\n]s",&Pid, Msg);
 
     int check = checkProcess(Pid);  //CHECK ITSELF !!!!!
     if(check == -1){
       printf("%d is not a named proecess in a readyQueue \n", Pid);
     }else{

     printf("CHECK PID:%d \n", Pid);
      
     Send(Pid, Msg);
     
     }
     
     
  }else if(userInput == 'R'){ //R: Receive
     int result = 0;
 
     result =  Receive();
     if(result == 1){
       printf("Receive success!!! \n");
     }else{ 
      printf("Recieve failure \n");
    } 

  }else if(userInput == 'Y'){ //Y: Reply
     int result = 0;
     int Pid = 0;
     char *Msg = malloc(sizeof(char)*40);

     printf("please input pid and Reply Msg you want to reply: \n");
     scanf("%d %[^\n]s", &Pid, Msg);
     
     result =  Reply(Pid, Msg);
     if(result == 1){
       printf("REPLY success!!! \n");
     }else{
       printf("REPLY failure \n");
     }

     
  }else if(userInput == 'N'){ //N: Nes Semaphore
     int id = -1;
     int value = -1;
     int result = 0;
     printf("please input id and value of New semaphore \n");
     scanf("%d %d", &id, &value);
     result = New_semaphore(id, value);
     if(result == 1){
       printf(" success New Semaphore \n");
     }else if(result == -1){
       printf(" Fail New Semaphore \n");
     }

  }else if(userInput == 'P'){
    int result = 0;
    int id = -1;
    printf("please input semaphore id you want to share: \n");
    scanf("%d", &id);
    result = Semaphore_P(id);
    if(result == 1){
       printf("semaphore_P SUCCESS \n");
    }else if(result == -1){
      printf("semaphore_P fail \n");
    }

  }else if(userInput == 'V'){
    int result = 0;
    int id = -1;
    printf("please input semaphore id you want to share: \n");
    scanf("%d", &id);
    result = Semaphore_V(id);
    if(result == 1){
      printf("semaphore_V SUCCESS \n");
    }else if(result == -1){
      printf("semaphore_V FAIL \n");
    }
  }else if(userInput == 'O'){ // O: End
     
      process = false;
      return 0;
  }  
 }
}

   //Create Queue  =========================================================================
   Queue* createQueue(int priority){
   Queue *QueueList[5];
   if(TotalQueueCnt < 5){
   Queue* queue = &QueueArray[priority];
   if( priority >= 0 && priority < 3){
      queue -> priority = priority;
   }else{
      queue -> priority = ' '; 
   }
   queue -> head = NULL;
   queue -> tail = NULL;
   queue -> processCnt = 0; 
   queue -> timeQuantum = 0; 
   TotalQueueCnt ++;

   QueueList[priority] = queue;
   return QueueList[priority]; 
   }
   else{
      return 0; 
   } 
}


int createProcess(int priority,Queue** readyQueue){
 
   MaxId ++; //pid
   PCB* process = (PCB*)malloc(sizeof(PCB));
   Queue* queue = readyQueue[priority];
   
   if( process == (PCB*)0){
      printf("error \n");
      return -1;  
   }
   
   //init
   if((readyQueue[0] -> init == NULL) && (readyQueue[1] -> init == NULL) && (readyQueue[2] -> init == NULL)){
      process -> pid = 0;
      queue -> init = process;
      MaxId --;
   }else{
      process -> pid = MaxId;
      
   }

   process -> priority = priority;
   process -> proState = 'R';
   process -> proc_message = NULL; 

   if(queue -> processCnt == 0){
      queue -> head = process;
      queue -> tail = process;
      process -> next = NULL;
      process -> prev = NULL;
   }else{
      process -> next = NULL;
      process -> prev = queue -> tail; 
      queue -> tail -> next = process;
      queue -> tail = process;  
   }

   queue -> processCnt = queue -> processCnt +1 ;
   queue -> run = NULL;
   process_run();
   return 0;
  }

  
int process_run(){ 

 for(int i = 0; i <3; i ++){
   if(readyQueue[i] != NULL){
      Queue* queue = readyQueue[i];
      PCB* process = queue -> head;
      PCB* temp = queue -> head;      
      if((*queue).processCnt>0){  
         if(queue -> run == NULL){
           
            //init
           if(queue -> init != NULL){
             //printf("queue -> init: %p", queue -> init);
             //printf("process:%p", process);
             //printf("processCnt:%d", (*queue).processCnt);
             if((queue -> init == process) && ((*queue).processCnt > 1)){
                //move the queue to the end of the queue
                printf("test init in process run \n");
                queue -> head = process -> next;
                process -> next -> prev = NULL;
                process -> prev = queue -> tail;
                process -> next = NULL;
                queue -> tail -> next = process;
                queue -> tail = process;
                process -> proState = 'R';
                
                temp = queue -> head;
             }
            }

            temp -> proState = 'A';
            queue -> run = temp;         
            for(int t = i+1 ; t <3; t++){    
               Queue* tempQueue = readyQueue[t];
               PCB* tempProcess = tempQueue -> head;
               for(int j =0; j < (*tempQueue).processCnt; j ++){
                  if(tempProcess -> proState == 'A'){
                     tempProcess -> proState = 'R';
                     tempQueue -> run = NULL;
                     return 0;
                  }
                  tempProcess = tempProcess -> next;
               }
            }
         }
      return queue -> run -> pid;
    }
   }
 }
 return -1;
}
  
//CHECK THE READYQUEUE WHEN ALL THE PROCESS IS KILLED OF THE QUEUE
int Kill(int pid){
   //INIT
   if(pid == 0){
     for(int i=0; i <5 ; i++){
        if(readyQueue[i] -> processCnt != 0){
           printf("initial process can not be killed until all process are expired \n");
           return -1;
        }
        if(proList[i] != NULL){
          if(proList[i] -> sem_list ->  processCnt != 0){
           printf("initial process can not be killed until all process are expired \n");
           return -1;
          }
        }
     }
   }

   for(int i=0; i <5; i++){
      if(readyQueue[i] != NULL){
         Queue* queue = readyQueue[i];
         PCB* process = queue -> head;
         for(int j=0; j< (*queue).processCnt; j++){
            if(process->pid == pid){         
               if((*queue).processCnt == 1){
                  queue -> head = NULL;
                  queue -> tail = NULL;
                  process -> next = NULL;
                  process -> prev = NULL;
                  free(process);
               }else if(queue -> head == process){   //PCB is head of the Queue
                  queue -> head = process -> next; 
                  process -> next -> prev = NULL;
                  process -> next = NULL;
                  free(process);
               }else if(queue -> tail == process){ //PCB is tail of the Queue
                  queue -> tail = process -> prev;
                  process -> prev -> next = NULL;
                  process -> prev = NULL;
                  free(process);
               }else{
                  process -> prev -> next = process -> next;
                  process -> next -> prev = process -> prev;
                  process -> next = NULL;
                  process -> prev = NULL;
                  free(process);
               }     
               queue -> processCnt = queue -> processCnt -1;
               queue -> run = NULL;
               process_run();
               return 1; // success
            }else{
              process = process->next;
           }
         }   
       }
      }
   return -1;
}

int Exit(){
   for(int i =0;i<3;i++){
      if(readyQueue[i] != NULL){
         Queue* queue = readyQueue[i];
         PCB* process = queue -> head;
         for(int j=0; j < (*queue).processCnt; j++){

          if(process->proState == 'A'){
            printf("Exit currently running process pid: %d \n",process->pid);
            Kill(process->pid);
            return 1;
          }
          else{
            process = process-> next;
          }
        }
      }
   } 
   return -1;
}

int Fork(){
   for(int i =0;i<3;i++){
      if(readyQueue[i] != NULL){
         Queue* queue = readyQueue[i];
         PCB* p_process = queue -> head;
         for(int j=0; j < (*queue).processCnt; j++){
          if(p_process->proState == 'A'){
            createProcess(p_process->priority,readyQueue);
            PCB* c_process = queue ->tail;
            printf("Fork process pid: %d \n",c_process->pid);
            return 1;
          }
          else{
            p_process = p_process-> next;
          }
        }
      }
   }
   return -1;
}

int Quantum(){
   int pid = 0;
   for(int i=0;i<3;i++){
      if(readyQueue[i] != NULL){
         Queue* queue = readyQueue[i];
         PCB* process = queue -> head;
         if(queue ->run != NULL){
            Exit();
            pid = process_run();
            printf("New process pid is:%d \n",pid);
            return pid;
         }
         else{
            pid = process_run();
         }
      }
   }
   return pid;
} 

int Send(int pid, char *msg){ //Send Msg to pid 
  //check the pid can be itself
  Queue* MsgSendQueue = readyQueue[3];
  printf("Send pid:%d \n", pid);
  printf("Send msg:%s \n", msg);

 //CHECK THE PID IN THE RECEIVE LIST
  for(int i=0; i<3; i++){
     Queue* queue = readyQueue[i];
     Queue* receive = readyQueue[4];
     PCB* receiver = receive -> head;
     PCB* run = queue -> head;
     if(run != NULL){ 

        //CHECK THERE IS PID IN THE RECEIVE QUEUE
        if((*receive).processCnt > 0){
           for(int i=0; i < (*receive).processCnt; i++){
              if(receiver -> pid == pid){
                 run -> proc_message = msg;
                 receiver -> proc_message = msg;
                 receiver -> proState = 'R';

                 //unblock the receiver
                 receive -> processCnt = receive -> processCnt -1;
                 if((*receive).processCnt == 0){
                    receive -> head = NULL;
                    receive -> tail = NULL;
                 }else if(receive -> head == receiver){
                    receive -> head = receiver -> next;
                    receiver -> next -> prev = NULL;
                 }else if(receive -> tail == receiver){
                    receive -> tail = receiver -> prev;
                    receiver -> prev -> next = NULL;
                 }else{
                    receiver -> prev -> next = receiver -> next;
                    receiver -> next -> prev = receiver -> prev;
                  }
                 
                //PUT THE RECEIVER IN THE READY QUEUE
                printf("receiver -> priority:%d \n", receiver -> priority);
                Queue* temp = readyQueue[receiver -> priority];
                if(temp -> processCnt == 0){
                   temp -> head = receiver;
                   temp -> tail = receiver;
                   receiver -> next = NULL;
                   receiver -> prev = NULL;
                }else{
                  receiver -> next = NULL;
                  receiver -> prev = temp -> tail; 
                  temp -> tail -> next = receiver;
                  temp -> tail = receiver;  
                }
               temp -> processCnt = temp -> processCnt +1 ;
               temp -> run = NULL;
               process_run();
               return 1;  
              }
             receiver = receiver -> next;
           }
         }

        //remove running process in the readyQueue 
        //INIT
        if(run -> pid == 0){
          printf("there is no adequate receiver and initial process can not be blocked \n");
          return -1;
        }
        run -> proc_message = msg;
        run -> proState = 'B';   
        if((*queue).processCnt > 1){        
            queue -> head = run -> next;
            run -> next -> prev = NULL;        
         }else{
            
            queue -> head = NULL;
            queue -> tail = NULL;
            //queue -> run = NULL;
         }
        //put the reuuning process in the send queue
        if(MsgSendQueue -> head == NULL){
           MsgSendQueue -> head = run;
           MsgSendQueue -> tail = run;
           run -> next = NULL;
           run -> prev = NULL; 
        }else{  
          run -> prev = MsgSendQueue -> tail;
          run -> next = NULL;
          MsgSendQueue -> tail -> next = run; 
          MsgSendQueue -> tail = run;
        }     
        queue -> run = NULL;
        queue -> processCnt = queue -> processCnt -1;
        run -> msgTo  = pid;
        MsgSendQueue -> processCnt = MsgSendQueue-> processCnt +1;
        process_run();
        return 1;
     }    
  }
   return -1;
} 

int Reply(int pid, char* msg){ //Y: Reply
  Queue* queue = readyQueue[3];
  PCB* process = queue -> head;
  for(int i =0; i < (*queue).processCnt; i ++){  
    if(process -> pid == pid){
       process -> proc_message = msg;
       //unblock the sender
       process -> proState = 'R';
       queue -> processCnt = queue -> processCnt -1;
      
       if((*queue).processCnt == 0){
          queue -> head = NULL;
          queue -> tail = NULL;
       }else if(queue -> head == process){
          queue -> head = process -> next;
          process -> next -> prev = NULL;
       }else if(queue -> tail == process){
          queue -> tail = process -> prev;
          process -> prev -> next = NULL;
       }else{
         process -> prev -> next = process -> next;
         process -> next -> prev = process -> prev;
       }
       //the sender go to the readyQueue
       Queue* temp = readyQueue[process -> priority];
       if(temp -> processCnt == 0){
          temp -> head = process;
          temp -> tail = process;
          process -> next = NULL;
          process -> prev = NULL;
       }else{
          process -> next = NULL;
          process -> prev = temp -> tail; 
          temp -> tail -> next = process;
          temp -> tail = process;  
       }
       temp -> processCnt = temp -> processCnt +1 ;
       process_run();
       return 1;     
            
     }
  }
  return -1;
}

//R: Receive Message
int Receive(){
  PCB* receiver = NULL;
  Queue* queue = NULL; 
  for(int i =0; i <3; i++){
    queue = readyQueue[i];
    if(queue -> run != NULL){
        receiver = queue -> run;
        break;
    }
  }  
  Queue* SendQueue = readyQueue[3];
  PCB* sender = SendQueue -> head;
  if(receiver != NULL && sender != NULL){
       for(int i =0; i< (*SendQueue).processCnt; i++){
         if(sender -> msgTo ==  receiver -> pid){
            printf("there is a message you to receive \n");
            receiver -> proc_message = sender -> proc_message; //should be deleted when it run
            return 1;         
            }
            sender = sender -> next;
      }
   }    
      //remove from the readyQueue
      
      //INIT
      if(receiver -> pid == 0){
          printf("there is no message to receive and initial process can not be blocked \n");
          return -1;
      }
      
      queue -> processCnt = queue -> processCnt -1;
      receiver -> proState = 'B';
      if((*queue).processCnt == 0){
            
            queue -> head = NULL;
            queue -> tail = NULL;
      }else if(receiver -> next == NULL){
            
            receiver -> prev -> next = NULL;
            queue -> tail = receiver -> prev;
      }else if(receiver -> prev == NULL){
            
            receiver -> next -> prev = NULL;
            queue -> head = receiver -> next;
      }else{
            
            receiver -> next -> prev = receiver -> prev;
            receiver -> prev -> next = receiver -> next;
      }
      //put it in the receive queue
      Queue* receive = readyQueue[4];
      if((*receive).processCnt > 0){
         
         receiver -> next = NULL;
         receiver -> prev = receive -> tail;
         receive -> tail -> next = receiver; 
         receive -> tail = receiver; 
         }else{
         
         receiver -> next = NULL;
         receiver -> prev = NULL;
         receive -> head = receiver;
         receive -> tail = receiver;
         }
         
         receive -> processCnt = receive -> processCnt +1 ;
         queue -> run = NULL;
         process_run();

         return -1;
}

//check the pid is named process 
int checkProcess(int pid){

  for(int i=0; i <5; i++){
   if(readyQueue[i] != NULL){
      Queue* queue = readyQueue[i];
      PCB* process = queue -> head;
      for(int i=0; i< (*queue).processCnt; i++){
          if(process->pid == pid){
            return 1;   
          }else{
          process = process -> next;
         }
      }
    }
  }
   return -1;
}

void TotalInfo(){
 for(int i=0; i<5; i++){
   if(readyQueue[i] != NULL){
      printf("------------------------------------\n");  
       if(i > -1 && i <3){
         printf("=======Ready Queue Statues==========\n");
       }else if(i == 3){
         printf("=======Send Queue Statues===========\n");
       }else{
         printf("=======Receive Queue Statues========\n");
       }
       printf("------------------------------------\n");   
     Queue* queue = readyQueue[i];
     PCB* temp = queue -> head;
     printf("\n");
     if( i > -1 && i <3){
     printf("Queue priority: %d \n", i);  
     }       
     printf("PCB Cnt:%d\n", (*queue).processCnt);
     
     for(int i=0; i < (*queue).processCnt ; i++){
       printf("pid:%d  \n", temp-> pid);
       printf("priority:%d  \n", temp-> priority);
       printf("proState:%c  \n", temp-> proState);
       printf("proc_message:%s \n", temp -> proc_message);
       printf("msgTo:%d \n", temp -> msgTo);
       printf("\n");
       temp = temp -> next;
     }
   }
  }

  Semaphore_List();
}

void Proc_info(int pid){
int check = 0;
   for(int i=0; i <3; i++){
      if(readyQueue[i] != NULL){
         Queue* queue = readyQueue[i];
         PCB* process = queue -> head;
         for(int j=0; j< (*queue).processCnt; j++){
            if(process->pid == pid){
               check = 1;
               printf("************************************\n");
               printf("============Process Info============ \n");
               printf("pid:%d \n",process->pid);
               printf("priority:%d \n", process->priority);
               printf("proState:%c \n", process->proState);
               printf("proc_message:%s \n",process->proc_message);
               printf("msgTo:%d \n",process->msgTo);
               printf("==================================== \n");
               printf("*************************************\n");
               printf("\n");
               break;
            }

           process = process -> next;
         }
      }
   } 
   if(check != 1){
   printf("there is no process of pid:%d \n", pid);  
   }     
}
void Semaphore_List(){
   printf("-----------------------------------\n"); 
   printf("==========Semaphore Statues========\n");
   for(int i = 0; i < 5; i++){
   Semaphore* sem = proList[i];
   if(proList[i] != NULL){
   printf("Semaphore_List pro_List[i]:%p \n", proList[i]);
   printf("Semaphore id:%d \n", i);
   printf("Semaphore value: %d \n", sem -> value);
   printf("Semaphore processCnt: %d \n", sem -> sem_list -> processCnt);
   printf("---------------------------------------\n"); 
   printf("\n");
   }
 }
}

int New_semaphore(int id, int value){
 Queue* QueueList[5];
 

 if(id < 0 || id > 4){
   printf(" id should be between 0 to 4 \n");
   return -1;
  } 

 if(value < 0){
   printf(" value should be higher or equal to 0 \n");
   return -1;
 }

 //check the number of semaphore  
 if(proList[id] == NULL){
   Semaphore* sem = &SemaphoreArray[id];
   proList[id] = sem;
   sem -> value = value;
   sem -> sem_list = &QueueArray[5+id];
   sem -> sem_list -> head = NULL;
   sem -> sem_list -> tail = NULL;
   sem -> sem_list -> processCnt = 0;
 }else{
  printf("there is already semaphore in the id: %d \n", id);
  return -1; 
 }

 Semaphore_List();
 return 1;
}

int Semaphore_P(int id){
  
  //CHECK THE SEMAPHORE EXISTS !!!!
  if(proList[id] == NULL){
    printf("there is no id of the semaphore \n");
    return -1;
  }

  Semaphore* sem = proList[id];
  Queue* queue = NULL;
  PCB* process = NULL;
  int S = sem -> value;

  for(int i=0; i<3 ; i++){
   if(readyQueue[i] != NULL){
      queue = readyQueue[i];
      if(queue -> run != NULL){
      process = queue -> run;
      break;
      }
    }
   }  

  if(process == NULL){
     printf("there is no running process \n");
     return -1;
  }
  
  if(S < 1){
     printf("semaphore test \n");

     //INIT
     if(process -> pid == 0){
        printf("there is no available semaphore and initial process can not be blocked \n");
        return -1;
      }

     process -> proState = 'B';
     //put it in the wait list
     if(sem -> sem_list -> processCnt == 0){
        sem -> sem_list -> head = process;
        sem -> sem_list -> tail = process;
     }else{
        sem -> sem_list -> tail -> next = process;
        process -> prev = sem -> sem_list -> tail;
        sem -> sem_list -> tail = process;   
      }
     
    sem -> sem_list -> processCnt = sem -> sem_list -> processCnt +1;

    //remove from the readyQueue     
    queue -> processCnt = queue -> processCnt -1;

   if((*queue).processCnt == 0){
             
       queue -> head = NULL;
       queue -> tail = NULL;
    }else if(process -> next == NULL){
             
        process -> prev -> next = NULL;
        queue -> tail = process -> prev;
     }else if(process -> prev == NULL){
             
        process -> next -> prev = NULL;
        queue -> head = process -> next;
     }else{
             
        process -> next -> prev = process -> prev;
        process -> prev -> next = process -> next;
     }
          
      queue -> run = NULL;
      process_run();
    }
 
   S --;
   sem -> value = S;
   return 1;
}

int Semaphore_V(int id){

  if(proList[id] == NULL){
    printf("there is no id of the semaphore \n");
    return -1;
  } 
 
  Semaphore* sem = proList[id];
  Queue* queue = NULL;
  PCB* process = NULL;
  int S = sem -> value;
  
  S++;
  sem -> value = S;

  if(S >0){

    if(sem -> sem_list -> processCnt >0){

    process = sem -> sem_list -> head;
    process -> proState = 'R';
    queue = readyQueue[process -> priority];
    //put out in the wait list
    if(sem -> sem_list -> processCnt == 1){
        sem -> sem_list -> head = NULL;
        sem -> sem_list -> tail = NULL;
    }else{
       process -> next -> prev = NULL;
       sem -> sem_list -> head = process -> next;
    }

    sem -> sem_list -> processCnt = sem -> sem_list -> processCnt -1;
   
    //put the process in the ready Queue
    if(queue -> processCnt == 0){
       queue -> head = process;
       queue -> tail = process;
       process -> next = NULL;
       process -> prev = NULL;
     }else{
       process -> next = NULL;
       process -> prev = queue -> tail; 
       queue -> tail -> next = process;
       queue -> tail = process;  
     }
  
     queue -> processCnt = queue -> processCnt +1 ;
     queue -> run = NULL;
     process_run();
    }
  }
   
   return 1;
}


