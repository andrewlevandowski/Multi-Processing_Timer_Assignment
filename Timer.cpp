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
#include <pthread.h>

using namespace std;


int _time;
bool handle = true;     // for signal handling

Timer::Timer(){}
Timer::~Timer(){}

void sigHandler(int s)      // used to handle signal
{
    handle = false;
}

bool Timer::setTime(int numArgs, const char* timeArg)
{
    try{
        _time = 25;      // default time if no arg present

        if(numArgs == 2)
            _time = stoi(timeArg);   // if arg passed in

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

void wallClock()
{
    time_t timeInfo;
    struct tm* lTime;
    
    char timearr [9];       // stores time output
	
    signal(SIGINT, sigHandler);     // catch signal from count down timer

    for(int i=0; handle; i++)       // loop wall clock output
    {
        timeInfo = time(NULL);          // get time output setup
        lTime = localtime(&timeInfo);
        strftime(timearr, 9, "%H:%M:%S", lTime);

        if(i == 0)
            printf ("Start Time: %s\n\n", timearr);
        else
            printf ("Time: %s\n", timearr);
            
        sleep(1);
    }

    printf("Alarm ringing!!\n");
    timeInfo = time(NULL);
    lTime = localtime(&timeInfo);
    strftime(timearr, 9, "%H:%M:%S", lTime);
    printf ("Final Time: %s\n", timearr);
}

void countDown(int start)
{
    int tempStart = start;      // save time passed in

    while(start > 0)
    {
        if(start == tempStart)
		    printf("Count Length: %d sec\n", start);
        else
            printf("Count Down: %d sec\n", start);
            
        start--;
		sleep(1);       // loop every second
    }
     
    printf("Count Down: %d sec\n", start);
    raise(SIGINT);      // raise signal to wall clock
}
    
void* threadRunner(void* thr_ptr)
{
    int thrID = (intptr_t) thr_ptr;

    if(thrID == 1)
        wallClock();
    if(thrID == 2)
        countDown(_time);
}

void threadCreator()
{
    int thr1_ID = 1;
    int thr2_ID = 2;

    pthread_t thread1;
    pthread_t thread2;

    pthread_create (&thread1, NULL, threadRunner, (void*)(intptr_t) thr1_ID);
    pthread_create (&thread2, NULL, threadRunner, (void*)(intptr_t) thr2_ID);

    pthread_join(thread1, NULL);
    pthread_join(thread2, NULL);
}
