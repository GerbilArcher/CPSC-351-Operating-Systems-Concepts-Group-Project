//=====================================================================
//CPSC-351-01(10369) pidCoding Group Assignment
//Main Portion - main.cpp File (File 1 of 1)
//Programmers:    GR, LG, AL, JM
//Term:           Summer 2021
//=====================================================================

#include <thread>
#include "generator.cpp"
#include "algorithms.cpp"
#include<iostream>
#include <fstream>



using namespace std;
// main function -
// where the execution of program begins
int main()
{

    Algorithms project;
    DATA_GENERATOR data;
    data.master();
    project.StartInternalThread();
    project.calcMinAvgWT();

    return 0;
}
