# makefile
#
# use make command under current directory

# Variable Definition
OBJS = MainFunction.o ScheduleUtility.o ProcessUtility.o QueueUtility.o DieWithMessage.o
CC = gcc
CFLAGS = -Wall -O -g
TARGET = ./Scheduler

# Compile .c file to .o file
%.o : %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Get all .c files
SOURCES = $(wildcard *.c)
# Replace .c file by .o file
OBJS = $(patsubst %.c, %.o, $(SOURCES))

# Link all .o files
$(TARGET) : $(OBJS)
	$(CC) $(OBJS) -o $(TARGET)
	chmod a+x $(TARGET)

# make clean command to delete all .o files and .exe file
clean:
	rm -rf *.o Scheduler
