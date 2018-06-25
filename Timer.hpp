/*
    Members: Cody Morgan, Andrew Levandowski
    Accounts: cssc1211 (location of assignment), cssc1214
    Class: CS 570, Summer 2018
    Assignment: Assignment 3, Multi-Processing Timer
    Filename: Timer.hpp
*/

using namespace std;


class Timer
{
    private:
        int _time;
        int p1[2];  // for pipe 1
        int p2[2];  // for pipe 2
        char endProc[2];    // holds end process string
        char endProcCmp[2]; // holds string to compare for end process
        void wallClock();
        void countDown(int start);
        
    public:
        Timer();
        ~Timer();
        bool setTime(int numArgs, const char* timeArg);
        void createPipes();
        void closePipes();
        void createProcesses();
        void closeProcesses();
};



