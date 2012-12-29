/////////////////////////////////////////////////////////////////////
/*
	File Name:		ProcessUtility.c
	Instructor:		Prof. Mohamed Zahran
	Grader:			Libin Lu
	Author:			Shen Li
	UID:			N14361265
	Department:		Computer Science
	Note:			This ProcessUtility.c file includes
					Get Process Information, Set Process Priority,
					Sort Process based on Priority,
					and Print Process Information Function.
*/
/////////////////////////////////////////////////////////////////////

///////////////HEADER FILES///////////////
#include "Scheduler.h"

///////////////FUNCTIONS///////////////
/*Initialize Process Information Function
  Variable Definition:
  -- stream: file stream for process information
  Return value: header pointer of process_info structure
*/
PROCESS_INFO *initProcess(FILE *stream){
	PROCESS_INFO	*header = (PROCESS_INFO*)malloc(sizeof(PROCESS_INFO));	//process_info structure header pointer
	PROCESS_INFO	*node;													//process_info structure node
	char			buffer[STRING_SIZE];									//each process information

	//Initialize buffer
	memset(buffer, 0, STRING_SIZE);
	//Initialize process_info header pointer
	header->id = -1;
	header->cpu_time = -1;
	header->running_time = -1;
	header->remain_time = -1;
	header->start_time = -1;
	header->finish_time = -1;
	header->io_time = -1;
	header->arrival_time = -1;
	header->priority = -1;
	header->next = NULL;
	
	//Let the node pointer point to the header
	node = header;
	//Get the request message from file stream
	while (fgets(buffer, STRING_SIZE, stream) != NULL){
		//Jump the blank line
		if ((strcmp(buffer, CRLF) != 0) || (strcmp(buffer, LF) != 0)){
			//Allocate the memory for new node
			node->next = (PROCESS_INFO*)malloc(sizeof(PROCESS_INFO));
			//Let the node pointer point to the current process
			node = node->next;
			//Assign variables(id, cpu_time, io_time, and arrival_time)
			sscanf(buffer, "%d %d %d %d", &node->id, &node->cpu_time, &node->io_time, &node->arrival_time);
			//Assign other variables
			node->running_time = 0;
			node->remain_time = node->cpu_time;
			node->start_time = node->arrival_time;
			node->finish_time = -1;
			node->priority = node->id;
			//Assign next pointer to NULL
			node->next = NULL;
		}
	}

	return header;
}

/*Set Process Priority Function
  Variable Definition:
  -- header: header pointer of process_info structure
  -- signal: signal that decide process priority
  Return value: NULL
*/
void setProcessPriority(PROCESS_INFO *header, const char signal){
	PROCESS_INFO	*node;		//process_info structure node
	
	//According to the signal, set the process priority value
	switch (signal){
		//ID
		case 'I':
			for (node = header; node != NULL; node = node->next){
				node->priority = node->id;
			}
			break;
		//Start Time
		case 'S':
			for (node = header; node != NULL; node = node->next){
				node->priority = node->start_time;
			}
			break;
		//CPU Time
		case 'R':
			for (node = header; node != NULL; node = node->next){
				node->priority = node->remain_time;
			}
			break;
		//IO Time
		case 'O':
			for (node = header; node != NULL; node = node->next){
				node->priority = node->io_time;
			}
		default:
			break;
	}

	return;
}

/*Sort Process Information Function
  Variable Definition:
  -- header: header pointer's pointer of process_info structure
  -- end: end pointer of process_info structure
  Return value: NULL
*/
void sortProcess(PROCESS_INFO **header, PROCESS_INFO *end){
	PROCESS_INFO	*right;				//right link header pointer
	PROCESS_INFO	**left_walk;		//left link pointer's pointer
	PROCESS_INFO	**right_walk;		//right link pointer's pointer
	PROCESS_INFO	*pivot;				//pivot pointer
	PROCESS_INFO	*old;				//old pointer
	int				count;				//total counter
	int				left_count;			//left link counter
	int				right_count;		//right link counter
	
	//Test whether the link is empty
	if (*header == end){
		return;
	}
	do{
		//Initialize the pointer variables
		pivot = *header;
		left_walk = header;
		right_walk = &right;
		//Initialize the counter variables
		left_count = right_count = 0;

		//Get the first node to be the comparison pivot
		for (old = (*header)->next; old != end; old = old->next){
			//Add the node which is smaller than pivot to the left process_info link
			if (old->priority < pivot->priority){
				++left_count;
				*left_walk = old;
				left_walk = &(old->next);
			}
			//Add the node which is biffer than pivot to the right process_info link
			else{
				++right_count;
				*right_walk = old;
				right_walk = &(old->next);
			}
		}
		//Combine the left link and right link
		*right_walk = end;
		*left_walk = pivot;
		pivot->next = right;
        //Continue sort left link and right link
		if (left_walk > right_walk){
			sortProcess(&(pivot->next), end);
			end = pivot;
			count = left_count;
		}
		else{
			sortProcess(header, pivot);
			header = &(pivot->next);
			count = right_count;
		}
	}while (count > 1);
}

/*Print Process Information Function
  Variable Definition:
  -- header: header pointer of process_info structure
  Return value: NULL
*/
void printProcess(PROCESS_INFO *header){
	PROCESS_INFO	*node;		//process_info structure node

	//Print the process information
	printf("<PID> <CPU Time> <I/O Time> <Arrival Time> <Priority>\n");
	for (node = header->next; node != NULL; node = node->next){
		printf("%4d    %4d      %4d         %4d         %4d\n", node->id, node->cpu_time, node->io_time, node->start_time, node->priority);
	}

	return;
}
