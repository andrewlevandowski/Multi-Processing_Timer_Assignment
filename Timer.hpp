/*
    Members: Cody Morgan, Andrew Levandowski
    Accounts: cssc1211 (location of assignment), cssc1214
    Class: CS 570, Summer 2018
    Assignment: Assignment 3, Multi-Processing Timer
    Filename: Timer.hpp
*/

using namespace std;

void* threadRunner(void* thr_ptr);
void threadCreator();

class Timer
{
    public:
        Timer();
        ~Timer();
        bool setTime(int numArgs, const char* timeArg);
};



