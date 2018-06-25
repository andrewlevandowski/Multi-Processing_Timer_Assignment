/*
    Members: Cody Morgan, Andrew Levandowski
    Accounts: cssc1211 (location of assignment), cssc1214
    Class: CS 570, Summer 2018
    Assignment: Assignment 3, Multi-Processing Timer
    Filename: Timer.cpp
*/

#include "Timer.hpp"
#include <stdio.h>
#include <time.h>
#include <unistd.h>
#include <string>
#include <string.h>
#include <stdlib.h>
#include <sys/wait.h>

using namespace std;


Timer::Timer(){}
Timer::~Timer(){}

bool Timer::setTime(int numArgs, const char* timeArg)
{
    try{
        Timer::_time = 25;      // default time if no arg present

        if(numArgs == 2)
            Timer::_time = stoi(timeArg);   // if arg passed in

        if(numArgs > 2)
        {
            printf("Error: only 2 arguments allowed. Exiting program...\n");
            return false;
        }
    }
    catch(const exception& e)
    {
        printf("Invalid argument - run again with a valid time\n");
        printf("Exiting program...\n");
        return false;
    }

    return true;
}
    

void Timer::createPipes()
{
    if (pipe(Timer::p1) == -1)
    {
        printf("Error creating pipe 1\n");
        exit(1);
    }
    
    if (pipe(Timer::p2) == -1)
    {
        printf("Error creating pipe 2\n");
        exit(1);
    }
}

void Timer::closePipes()
{   
    if (close(Timer::p1[0]) == -1)
    {
        printf("Error closing pipe 1\n");
        exit(EXIT_FAILURE);
    }
    
    if (close(Timer::p1[1]) == -1)
    {
        printf("Error closing pipe 1\n");
        exit(EXIT_FAILURE);
    }
    
    if (close(Timer::p2[0]) == -1)
    {
        printf("Error closing pipe 2\n");
        exit(EXIT_FAILURE);
    }
    
    if (close(Timer::p2[1]) == -1)
    {
        printf("Error closing pipe 2\n");
        exit(EXIT_FAILURE);
    }
}

void Timer::createProcesses()
{
    switch (fork())     // create wall clock child process
    {
        case -1:
            printf("Fork error: child 1\n");
            exit(1);
            
        case 0:
            wallClock();        // get wall clock time
            exit(1);
            
        default:
            break;
    }
    
    switch (fork())     // create count down child process
    {
        case -1:
            printf("Fork error: child 2\n");
            exit(1);
            
        case 0:
            countDown(Timer::_time);    // count down time
            exit(1);
            
        default:
            break;
    }
}

void Timer::closeProcesses()
{    
    if (wait(NULL) == -1)       // parent waits for child to terminate
    {
        printf("Error wating, no child process is ended!\n");
        exit(EXIT_FAILURE);
    }

    if (wait(NULL) == -1)      // parent waits for child to terminate
    {
        printf("Error wating, no child process is ended!\n");
        exit(EXIT_FAILURE);
    }
    
	printf("Parent process terminating, exiting program...\n");
    exit(EXIT_SUCCESS);
}

void Timer::countDown(int start)
{
	strcpy(Timer::endProc, "1");    // signal string to end process
    
    if (close(Timer::p2[0]) == -1)
    {
        printf("Error closing pipe 2\n");
        _exit(1);
    }
    
	if (close(Timer::p1[1]) == -1)
    {
        printf("Error closing pipe 1\n");
        _exit(1);
    }
    
    if (close(Timer::p1[0]) == -1)
    {
        printf("Error closing pipe 1\n");
        _exit(1);
    }
    
    int tempStart = start;      // save time passed in

    while(start > 0)
    {
        if(start == tempStart)
		    printf("Count Length: %d sec\n", start);
        else
            printf("Count Down: %d sec\n", start);
            
        write(Timer::p2[1], "0", 2);    // signal wall clock to continue
        start--;
		sleep(1);       // loop every second
    }
    
    write(Timer::p2[1], Timer::endProc, sizeof(Timer::endProc));        // signal wall clock to stop
    
    if (close(Timer::p2[1]) == -1)
    {
        printf("Error closing pipe 2");
        _exit(EXIT_FAILURE);
    }
    
    printf("Count Down: %d sec\n", start);
    exit(1);
}

void Timer::wallClock()
{
	strcpy(Timer::endProcCmp, "1");     // string to compare with end process string
    
    time_t timeInfo;
    struct tm* lTime;
    
    if (close(Timer::p1[0]) == -1)
    {
        printf("Error closing pipe 1\n");
        _exit(1);
    }
    
	if (close(Timer::p2[1]) == -1)
    {
        printf("Error closing pipe 2\n");
        _exit(1);
    }
    
	if (close(Timer::p1[1]) == -1)
    {
        printf("Error closing pipe 1\n");
        _exit(1);
    }
    
    char timearr [9];       // stores time output
	
    for(int i=0;;i++)       // loop wall clock output
    {
        
        timeInfo = time(NULL);
        lTime = localtime(&timeInfo);
		
		int status = read(Timer::p2[0], Timer::endProc, 2);     // get signal from count down process
        int cmp = strcmp(Timer::endProcCmp, Timer::endProc);    // compare signal strings
        
		if(cmp != 0)       // still counting down
		{
            strftime(timearr, 9, "%H:%M:%S", lTime);

            if(i == 0)
                printf ("Start Time: %s\n\n", timearr);
            else
                printf ("Time: %s\n", timearr);
                
            sleep(1);
		}
		else
        {
            if (close(Timer::p2[0]) == -1)
            {
                printf("Error closing pipe 2\n");
                _exit(1);
            }
            
            printf("Alarm ringing!!\n");
            strftime(timearr, 9, "%H:%M:%S", lTime);
            printf ("Final Time: %s\n", timearr);
            exit(1);
        }
    }
}

