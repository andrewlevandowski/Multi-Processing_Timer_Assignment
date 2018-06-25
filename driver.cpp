/*
    Members: Cody Morgan, Andrew Levandowski
    Accounts: cssc1211 (location of assignment), cssc1214
    Class: CS 570, Summer 2018
    Assignment: Assignment 3, Multi-Processing Timer
    Filename: driver.cpp
*/

#include "Timer.hpp"

using namespace std;


int main(int argc, const char * argv[])
{

    Timer* oTimer = new Timer;      // create Timer object

    if(!oTimer->setTime(argc, argv[1]))     // validates command line arg
    {
        delete oTimer;
        return 0;
    }
    
    oTimer->createPipes();
    oTimer->createProcesses();
    oTimer->closePipes();
    oTimer->closeProcesses();
    
    delete oTimer;
    return 0;
}

