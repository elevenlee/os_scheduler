////////////////////////////////////////////////////////////////
/*
	File Name:		Scheduler.h
	Instructor:		Prof. Mohamed Zahran
	Grader:			Libin Lu
	Author:			Shen Li
	UID:			N14361265
	Department:		Computer Science
	Note:			This Scheduler.h file includes
					HEADER FILES, MACRO, STRUCT DEFINITION,
					GLOBAL VARIABLE AND FUNCTION DECLARATION.
*/
////////////////////////////////////////////////////////////////

///////////////PRECOMPILER///////////////
#ifndef	SCHEDULER_H_
#define SCHEDULER_H_

///////////////DEBUG///////////////
//#define DEBUG 1
#ifdef DEBUG
	#define DEBUG_PRINT printf("%s-%s:%d:", __FILE__, __FUNCTION__, __LINE__)
#else
	#define DEBUG_PRINT
#endif

///////////////HEADER FILES///////////////
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

///////////////MACRO///////////////
#define	FILE_NAME	"output.txt"
#define	CRLF		"\r\n"
#define LF			"\n"

///////////////GLOBAL VARIABLE///////////////
enum size_constants{
	QUANTUM_SIZE	=	2,
	QUEUES_SIZE		=	5,
	STRING_SIZE		=	64,
};

///////////////STRUCT DEFINITION///////////////
/*process information structure*/
typedef struct _process_info{
	int		id;						//id number
	int		cpu_time;				//CPU time
	int		running_time;			//running time
	int		remain_time;			//remain time
	int		start_time;				//start time
	int		finish_time;			//finish time
	int		io_time;				//I/O time
	int		arrival_time;			//arrival time
	int		priority;				//process priority
	struct _process_info	*next;	//next pointer
}PROCESS_INFO;

/*status queues structure*/
/*	queues[0]: unarrival
	queues[1]: blocked
	queues[2]: ready
	queues[3]: running
	queues[4]: finish
*/
typedef struct _status_queues{
	PROCESS_INFO *queues[QUEUES_SIZE];	//status queues pointer array
}STATUS_QUEUES;

///////////////FUNCTION DECLARATION///////////////
/*ScheduleUtility.c*/
void	firstComeFirstServed(FILE *stream, PROCESS_INFO *header, const char signal);
void	roundRobin(FILE *stream, PROCESS_INFO *header, const char signal);
void	shortestRemainingJobFirst(FILE *stream, PROCESS_INFO *header, const char signal);
/*ProcessUtility.c*/
PROCESS_INFO	*initProcess(FILE *stream);
void	setProcessPriority(PROCESS_INFO *header, const char signal);
void	sortProcess(PROCESS_INFO **header, PROCESS_INFO *end);
void	printProcess(PROCESS_INFO *header);
/*QueueUtility.c*/
STATUS_QUEUES	*initStatusQueues(PROCESS_INFO *header, const char signal);
bool	notEmptyQueue(PROCESS_INFO *front);
bool	emptyStatusQueues(STATUS_QUEUES *schedule);
void	insertInQueue(PROCESS_INFO *front, PROCESS_INFO *node, const char signal);
PROCESS_INFO	*deleteInQueue(PROCESS_INFO *front);
PROCESS_INFO	*peekInQueue(PROCESS_INFO *front);
void	clearInQueue(PROCESS_INFO *front);
void	clearStatusQueues(STATUS_QUEUES *schedule);
void	printQueuesSnapshot(FILE *stream, STATUS_QUEUES *schedule, int count);
void	printQueuesStatistics(FILE *stream, STATUS_QUEUES *schedule, int count, int run);
/*DieWithMessage.c*/
void	dieWithUserMessage(const char *message, const char *detail);
void	dieWithSystemMessage(const char *message);

#endif //SCHEDULER_H
