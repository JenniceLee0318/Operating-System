
Structure 
1.PCB
	pid: process id
	priority: process priority
	proState: process State \{R: Ready, A: Running, B: Blocked\}\
	proc_message: message 
	msgTo: pid that the message is sent to
	next: points to next PCB or Process
	prep: points to previous PCB or Process

2.Queue
	head: head of the queue for PCB
	tail: tail of the queue for PCB
	run: currently running PCB
	init: contains init process {init process has pid:0,it is created automatically when the program runs}
	priority: Each queue has different priority {0:High, 1:Medium, 2:Low\}
	processCnt: number of PCB or Process in the queue

ReadyQueue is consist of 5 queues. Queue[0], Queue[1], Queue[2] has processes with different 
priorities of 0,1,2. Queue[3] has processes waiting on the send operation and Queue[5] had processes waiting on a receive operation

3.Semaphore
	Newseamaphore:
	-It can have ID from 0 to 4 and the values have to be bigger than 0
	-each 5 semaphore is referenced by the proList[I] where i values represent the ID 
	-Semaphore has its own value and sem_list structure which contains blocked wait processes

	semaphore_P:
	-it checks if the user input is available as the semaphore
	-it checks if there is any running process
	-if items in the semaphore of the current id < 1, that run process will be blocked and queued into sem_list
	-value of S will decrement

	semaphore_V:
	-it checks if the user input is available as the semaphore
	-S++
	-if S > 0, the blocked process residing in the sem_list will be removed and it will be queued back to its original priority queue
	
4. Extra functions
Main(): showcase different input cases for the each PCB control 
Process_run(): run the highest priority process in the queue
checkProcess(int pid): check if user input PID exist in the queue
Semaphore_List(): print the semaphore list information
