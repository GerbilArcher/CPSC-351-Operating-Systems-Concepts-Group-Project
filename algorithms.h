//=====================================================================
//CPSC-351-01(10369) pidCoding Group Assignment
//Sorting Algorithm Portion - algorithms.h File (File 1 of 2)
//Programmers:    GR, LG, AL, JM
//Term:           Summer 2021
//=====================================================================

#pragma once

#include<iomanip>
#include<mutex>

#include "generator.h"

mutex mtx;

double PriorityNPAvgWT;
double PriorityPAvgWT;
double SJFNPAvgWT;
double SJFPAvgWT;
double FCFSAvgWT;
double RRAvgWT;

class Algorithms
{
private:
public:
    Algorithms(); //Constructor
    ~Algorithms(); // Destructor

    //Class member function declarations
    void StartInternalThread(); // Runs all functions in threads.
    void priorityNP(int globalArray[][MAX_COLUMNS], int processAmount);
    void priorityP(int globalArray[][MAX_COLUMNS], int processAmount);
    void SJFNP(int globalArray[][MAX_COLUMNS], int processAmount);
    void SJFP(int globalArray[][MAX_COLUMNS], int processAmount);
    void FCFS(int globalArray[][MAX_COLUMNS], int processAmount);
    void RR(int globalArray[][MAX_COLUMNS], int processAmount);
    void calcMinAvgWT();
};
