///////////////////////////////////////////////
/*
	File Name:		MainFunction.c
	Instructor:		Prof. Mohamed Zahran
	Grader:			Libin Lu
	Author:			Shen Li
	UID:			N14361265
	Department:		Computer Science
	Note:			This MainFunction.c file
					includes Main Function.
*/
///////////////////////////////////////////////

///////////////HEADER FILES///////////////
#include "Scheduler.h"

///////////////FUNCTIONS///////////////
/*Main Function
  Variable Definition:
  -- argc: the number of command arguments
  -- argv[]: each vairable of command arguments(argv[0] is the path of execution file forever)
  Return Value: client exit number
*/
int main(int argc, char *argv[]){
	//Test for correct number of arguments
	if (argc < 3 || argc > 4){
		dieWithUserMessage("Parameter(s)", "<Input file name> <Schedule method> [<Output file name>]");
	}

	FILE	*in_channel;											//read file stream
	FILE	*out_channel;											//write file stream
	char	*input_file_name = argv[1];								//input file name
	char	*output_file_name = (argc == 4)? argv[3]:FILE_NAME;		//output file name
	int		method = atoi(argv[2]);									//schedule method

	//Open file for read process information
	if ((in_channel = fopen(input_file_name, "r")) == NULL){
		dieWithUserMessage("fopen() failed!", "Cannot open file to read process information!");
	}
	//Open file for write scheduler result(snapshot and statistics)
	if ((out_channel = fopen(output_file_name, "w")) == NULL){
		dieWithUserMessage("fopen() failed!", "Cannot open file to write schedule statistics!");
	}
	// According to method, start to schedule process
	switch (method){
		//First come first served
		case 1:
			firstComeFirstServed(out_channel, initProcess(in_channel), 'S');
			break;
		//Round-Robin with quantum 2
		case 2:
			roundRobin(out_channel, initProcess(in_channel), 'I');
			break;
		//Shortest remaining job first
		case 3:
			shortestRemainingJobFirst(out_channel, initProcess(in_channel), 'R');
			break;
		//Method could not allowed
		default:
			dieWithUserMessage("Method not allowed!", "Please input 1, 2 or 3!");
			break;
	}

	fprintf(stdout, "Schedule success! The result is in %s file.%s", output_file_name, LF);
	//Close read file
	fclose(in_channel);
	//Close write file
	fclose(out_channel);

	return 1;
}
