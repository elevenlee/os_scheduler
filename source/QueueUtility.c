////////////////////////////////////////////////////////////////
/*
	File Name:		QueueUtility.c
	Instructor:		Prof. Mohamed Zahran
	Grader:			Libin Lu
	Author:			Shen Li
	UID:			N14361265
	Department:		Computer Science
	Note:			This QueueUtility.c file includes
					Handle Queue(s) Function.
*/
////////////////////////////////////////////////////////////////

///////////////HEADER FILES///////////////
#include "Scheduler.h"

///////////////FUNCTIONS///////////////
/*Initialize status_queues structure Function
  Variable Definition:
  -- header: header pointer of process_info structure
  -- signal: signal that decide process priority
  Return value: status_queues structure pointer
*/
STATUS_QUEUES *initStatusQueues(PROCESS_INFO *header, const char signal){
	STATUS_QUEUES	*schedule = (STATUS_QUEUES*)malloc(sizeof(STATUS_QUEUES));	//status_queues structure
	int				count;														//counter
	
	//Sort the process by id number
	sortProcess(&header, NULL);
	//Set the process priority as start_time
	setProcessPriority(header, 'S');
	//Re-sort the process by start_time
	sortProcess(&header, NULL);
	//Set the process priority as "signal" decided
	setProcessPriority(header, signal);
	
	//Initialize the unarrival queue
	schedule->queues[0] = header;
	//Initialize the other four queues
	for (count = 1; count < QUEUES_SIZE; count++){
		//Allocate the memory for new queue front pointer
		schedule->queues[count] = (PROCESS_INFO*)malloc(sizeof(PROCESS_INFO));
		//Assign the front's next as NULL(empty queue)
		schedule->queues[count]->next = NULL;
	}

	return schedule;
}

/*Test Queue Not Empty Function
  Variable Definition:
  -- front: queue's front pointer
  Return value: if the queue is not empty, return true; else return false
*/
bool notEmptyQueue(PROCESS_INFO *front){
	//Test the queue's front pointer
	if (front->next == NULL){
		return false;
	}

	return true;
}

/*Test All Queues are Empty Function
  Variable Definition:
  -- schedule: status_queues structure pointer
  Return value: if all queues are empty, return true; else return false
*/
bool emptyStatusQueues(STATUS_QUEUES *schedule){
	int		count = 0;		//counter

	//Test all queues except "finish" queue
	while (count < QUEUES_SIZE - 1){
		if (notEmptyQueue(schedule->queues[count])){
			return false;
		}
		count++;
	}

	return true;
}

/*Insert a Element in Queue Function
  Variable Definition:
  -- front: queue front pointer
  -- node: process_info structure need to insert
  -- signal: signal that decide process priority
  Return value: NULL
*/
void insertInQueue(PROCESS_INFO *front, PROCESS_INFO *node, const char signal){
	PROCESS_INFO	*temp_before;	//the location before the insert node
	PROCESS_INFO	*temp;			//the location after the insert node

	//Set the process priority as "signal" decided
	setProcessPriority(front->next, signal);
	//Set the insert node's priority as "signal" decided
	setProcessPriority(node, signal);

	//Test the queue is empty
	if (front->next == NULL){
		//Insert the node after the front pointer
		front->next = node;
		node->next = NULL;
	}
	//Initialize process_info structure temp
	temp = front->next;
	temp_before = front;
	//Find the location which the node insert
	while (temp != NULL){
		//Compare priority of node and element in queue
		if (node->priority < temp->priority){
			//If smaller, then find it
			break;
		}
		else if (node->priority == temp->priority){
			//If equal, then compare the id number
			while ((temp != NULL) && (node->priority == temp->priority)){
				if (node->id < temp->id){
					break;
				}
				//Move the pointer to the next element in queue
				temp_before = temp;
				temp = temp->next;
			}
			break;
		}
		//Move the pointer to the next element in queue
		temp_before = temp;
		temp = temp->next;
	}
	//Insert the node
	temp_before->next = node;
	if (temp != NULL){
		node->next = temp;
	}
	else{
		node->next = NULL;
	}

	return;
}

/*Delete the First Element in Queue Function
  Variable Definition:
  -- front: queue front pointer
  Return value: first element in queue
*/
PROCESS_INFO *deleteInQueue(PROCESS_INFO *front){
	PROCESS_INFO	*node;		//process_info structure node

	//Test the empty queue
	if (front->next == NULL){
		dieWithUserMessage("deleteInQueue() failed!", "Cannot delete a element in the queue!");
	}
	//Set the node as the front element of queue
	node = front->next;
	//Set the front pointer
	if (node->next != NULL){
		front->next = node->next;
	}
	else{
		front->next = NULL;
	}
	//Set the node->next value as NULL
	node->next = NULL;

	return node;
}

/*Get the First Element in Queue Function
  Variable Definition:
  -- front: queue front pointer
  Return value: first element in queue
*/
PROCESS_INFO *peekInQueue(PROCESS_INFO *front){
	//Test the empty queue
	if (front->next == NULL){
		return NULL;
	}
	
	return front->next;
}

/*Clear all elements in Queue Function
  Variable Definition:
  -- front: queue front pointer
  Return value: NULL
*/
void clearInQueue(PROCESS_INFO *front){
	PROCESS_INFO	*node;		//process_info structure node

	//Set the node as the front element in the queue
	node = front->next;
	//Delete all element in the queue
	while (node != NULL){
		front->next = node->next;
		free(node);
		node = front->next;
	}
	//Set the queue as empty
	front->next = NULL;

	return;
}

/*Clear all elements in status_queues structure Function
  Variable Definition:
  -- schedule: status_queues structure pointer
  Return value: NULL
*/
void clearStatusQueues(STATUS_QUEUES *schedule){
	int		count;		//counter

	for (count = 0; count < QUEUES_SIZE; count++){
		//Clear all elements in queues
		clearInQueue(schedule->queues[count]);
		//Clear front pointer of each queue
		free(schedule->queues[count]);
	}
	//Clear status_queues structure
	free(schedule);

	return;
}

/*Print Process Snapshot Function
  Variable Definition:
  -- stream: file stream for schedule result
  -- schedule: status_queues structure pointer
  -- count: total time
  Return value: NULL
*/
void printQueuesSnapshot(FILE *stream, STATUS_QUEUES *schedule, int count){
	PROCESS_INFO	*node;		//process_info structure node

	//Output the time
	fprintf(stream, "%2d ", count);
	if (notEmptyQueue(schedule->queues[3])){
		//Output the running process
		fprintf(stream, "%2d: running	", schedule->queues[3]->next->id);
		//Plus the process running_time
		schedule->queues[3]->next->running_time++;
		//Minus the process remain_time
		schedule->queues[3]->next->remain_time--;
	}
	if (notEmptyQueue(schedule->queues[2])){
		for (node = schedule->queues[2]->next; node != NULL; node = node->next){
			//Output the ready process
			fprintf(stream, "%2d: ready	", node->id);
			//Set the process running_time as 0
			node->running_time = 0;
		}
	}
	if (notEmptyQueue(schedule->queues[1])){
		for (node = schedule->queues[1]->next; node != NULL; node = node->next){
			//Output the blocked process
			fprintf(stream, "%2d: blocked	", node->id);
			//Set the process running_time as 0
			node->running_time = 0;
			//Minus the process io_time
			node->io_time--;
		}
	}
	if (notEmptyQueue(schedule->queues[0])){
		for (node = schedule->queues[0]->next; node != NULL; node = node->next){
			//NOT output the unarrival process
			//Minus the process arrival_time
			node->arrival_time--;
		}
	}
	fprintf(stream, "%s", LF);

	return;
}

/*Print Process Statistics Function
  Variable Definition:
  -- stream: file stream for schedule result
  -- schedule: status_queues structure pointer
  -- count: total time
  -- run: running time
  Return value: NULL
*/
void printQueuesStatistics(FILE *stream, STATUS_QUEUES *schedule, int count, int run){
	PROCESS_INFO	*node;		//process_info structure node

	//Output finish time
	fprintf(stream, "%sFinish time: %2d%s", LF, count - 1, LF);
	//Output CPU utilization
	fprintf(stream, "CPU Utilization: %.4f%s", (double)run/count, LF);
	for (node = schedule->queues[4]->next; node != NULL; node = node->next){
		//Output each process's turnaround time
		fprintf(stream, "Turnaround process %2d: %2d%s", node->id, node->finish_time - node->start_time, LF);
	}

	return;
}
