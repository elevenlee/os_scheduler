//////////////////////////////////////////////////////////////////////////
/*
	File Name:		SceduleUtility.c
	Instructor:		Prof. Mohamed Zahran
	Grader:			Libin Lu
	Author:			Shen Li
	UID:			N14361265
	Department:		Computer Science
	Note:			This ScheduleUtility.c file includes
					First Come First Served Function, Round-Robin
					Function, and Shortest Remaining Job First Function
*/
//////////////////////////////////////////////////////////////////////////

///////////////HEADER FILES///////////////
#include "Scheduler.h"

///////////////FUNCTIONS///////////////
/*First Come First Served Function
  Variable Definition:
  -- stream: file stream for schedule result
  -- header: header pointer of process_info structure
  -- signal: signal that decide process priority
  Return value: NULL
*/
void firstComeFirstServed(FILE *stream, PROCESS_INFO *header, const char signal){
	STATUS_QUEUES	*schedule = initStatusQueues(header, signal);	//status_queues structure node
	PROCESS_INFO	*temp;											//process_info structure temp
	int				time_count = 0;									//total time
	int				running_count = 0;								//running time

	//Start schedule
	while (true){
		//Running queue has element
		if (notEmptyQueue(schedule->queues[3])){
			//Delete the first element in running queue
			temp = deleteInQueue(schedule->queues[3]);
			//Process need to handle I/O
			if (temp->remain_time == temp->cpu_time/2){
				insertInQueue(schedule->queues[1], temp, 'O');
			}
			//Process has finished
			else if (temp->remain_time == 0){
				temp->finish_time = time_count;
				insertInQueue(schedule->queues[4], temp, 'I');
			}
			//Process to ready queue
			else{
				insertInQueue(schedule->queues[2], temp, signal);
			}
		}
		//Get arrival process(es) and insert them to ready queue
		while (notEmptyQueue(schedule->queues[0]) && (schedule->queues[0]->next->arrival_time == 0)){
			temp = deleteInQueue(schedule->queues[0]);
			insertInQueue(schedule->queues[2], temp, signal);
		}
		//Get process(es) has handle I/O finish and insert them to ready queue
		while (notEmptyQueue(schedule->queues[1]) && (schedule->queues[1]->next->io_time == 0)){
			temp = deleteInQueue(schedule->queues[1]);
			insertInQueue(schedule->queues[2], temp, signal);
		}
		//Get the first element in ready queue and insert it to running queue
		if (notEmptyQueue(schedule->queues[2])){
			temp = deleteInQueue(schedule->queues[2]);
			insertInQueue(schedule->queues[3], temp, signal);
		}

		//Test whether scheduler is finished
		if (emptyStatusQueues(schedule)){
			break;
		}
		else if (notEmptyQueue(schedule->queues[3])){
			running_count++;
		}
		//Output schedule snapshot
		printQueuesSnapshot(stream, schedule, time_count);
		//Plus total time
		time_count++;
	}
	//Output schedule statistics
	printQueuesStatistics(stream, schedule, time_count, running_count);
	//Free all elements in queues
	clearStatusQueues(schedule);

	return;
}

/*Round-Robin Function
  Variable Definition:
  -- stream: file stream for schedule result
  -- header: header pointer of process_info structure
  -- signal: signal that decide process priority
  Return value: NULL
*/
void roundRobin(FILE *stream, PROCESS_INFO *header, const char signal){
	STATUS_QUEUES	*schedule = initStatusQueues(header, signal);	//status_queues structure node
	PROCESS_INFO	*temp;											//process_info structure temp
	PROCESS_INFO	*temp_next;										//process_info structure temp_next
	int				time_count = 0;									//total time
	int				running_count = 0;								//running time

	//Start schedule
	while (true){
		//Running queue has element
		if (notEmptyQueue(schedule->queues[3])){
			//Delete the first element in running queue
			temp = deleteInQueue(schedule->queues[3]);
			//Process need to handle I/O
			if (temp->remain_time == temp->cpu_time/2){
				insertInQueue(schedule->queues[1], temp, 'O');
			}
			//Process has finished
			else if (temp->remain_time == 0){
				temp->finish_time = time_count;
				insertInQueue(schedule->queues[4], temp, 'I');
			}
			//Process has running QUANTUM time
			else if (temp->running_time == QUANTUM_SIZE){
				insertInQueue(schedule->queues[2], temp, signal);
			}
			//Process to running queue
			else{
				insertInQueue(schedule->queues[3], temp, signal);
			}
		}
		//Get arrival process(es) and insert them to ready queue
		while (notEmptyQueue(schedule->queues[0]) && (schedule->queues[0]->next->arrival_time == 0)){
			temp = deleteInQueue(schedule->queues[0]);
			insertInQueue(schedule->queues[2], temp, signal);
		}
		//Get process(es) has handle I/O finish and insert them to ready queue
		while (notEmptyQueue(schedule->queues[1]) && (schedule->queues[1]->next->io_time == 0)){
			temp = deleteInQueue(schedule->queues[1]);
			insertInQueue(schedule->queues[2], temp, signal);
		}
		//Get the first or second element in ready queue and insert it to running queue
		if (notEmptyQueue(schedule->queues[2]) && !notEmptyQueue(schedule->queues[3])){
			//Delete the first element in ready queue
			temp = deleteInQueue(schedule->queues[2]);
			//Process NOT run QUANTUM time just now
			if (temp->running_time != QUANTUM_SIZE){
				insertInQueue(schedule->queues[3], temp, signal);
			}
			//Only one process in ready queue
			else if (!notEmptyQueue(schedule->queues[2])){
				temp->running_time = 0;
				insertInQueue(schedule->queues[3], temp, signal);
			}
			//Process has ran QUANTUM time should queue again
			else{
				temp_next = deleteInQueue(schedule->queues[2]);
				insertInQueue(schedule->queues[3], temp_next, signal);
				insertInQueue(schedule->queues[2], temp, signal);
			}
		}

		//Test whether scheduler is finished
		if (emptyStatusQueues(schedule)){
			break;
		}
		else if (notEmptyQueue(schedule->queues[3])){
			running_count++;
		}
		//Output schedule snapshot
		printQueuesSnapshot(stream, schedule, time_count);
		//Plus total time
		time_count++;
	}
	//Output schedule statistics
	printQueuesStatistics(stream, schedule, time_count, running_count);
	//Free all elements in queues
	clearStatusQueues(schedule);

	return;
}

/*Shortest Remaining Job First Function
  Variable Definition:
  -- stream: file stream for schedule result
  -- header: header pointer of process_info structure
  -- signal: signal that decide process priority
  Return value: NULL
*/
void shortestRemainingJobFirst(FILE *stream, PROCESS_INFO *header, const char signal){
	STATUS_QUEUES	*schedule = initStatusQueues(header, signal);		//status_queues structure node
	PROCESS_INFO	*temp;												//process_info structure temp
	int				time_count = 0;										//total time
	int				running_count = 0;									//running time

	//Start schedule
	while (true){
		//Running queue has element
		if (notEmptyQueue(schedule->queues[3])){
			//Delete the first element in running queue
			temp = deleteInQueue(schedule->queues[3]);
			//Process need to handle I/O
			if (temp->remain_time == temp->cpu_time/2){
				insertInQueue(schedule->queues[1], temp, 'O');
			}
			//Process has finished
			else if (temp->remain_time == 0){
				temp->finish_time = time_count;
				insertInQueue(schedule->queues[4], temp, 'I');
			}
			//Process to ready queue
			else{
				insertInQueue(schedule->queues[2], temp, signal);
			}
		}
		//Get arrival process(es) and insert them to ready queue
		while (notEmptyQueue(schedule->queues[0]) && (schedule->queues[0]->next->arrival_time == 0)){
			temp = deleteInQueue(schedule->queues[0]);
			insertInQueue(schedule->queues[2], temp, signal);
		}
		//Get process(es) has handle I/O finish and insert them to ready queue
		while (notEmptyQueue(schedule->queues[1]) && (schedule->queues[1]->next->io_time == 0)){
			temp = deleteInQueue(schedule->queues[1]);
			insertInQueue(schedule->queues[2], temp, signal);
		}
		//Get the first element in ready queue and insert it to running queue
		if (notEmptyQueue(schedule->queues[2])){
			temp = deleteInQueue(schedule->queues[2]);
			insertInQueue(schedule->queues[3], temp, signal);
		}

		//Test whether scheduler is finished
		if (emptyStatusQueues(schedule)){
			break;
		}
		else if (notEmptyQueue(schedule->queues[3])){
			running_count++;
		}
		//Output schedule snapshot
		printQueuesSnapshot(stream, schedule, time_count);
		//Plus total time
		time_count++;
	}
	//Output schedule statistics
	printQueuesStatistics(stream, schedule, time_count, running_count);
	//Free all elements in queues
	clearStatusQueues(schedule);

	return;
}
