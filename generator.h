//=====================================================================
//CPSC-351-01(10369) pidCoding Group Assignment
//Data Generation Portion - generator.h File (File 1 of 2)
//Programmer:     GR
//Group Members:  LG, AL, JM
//Term:           Summer 2021
//=====================================================================

#pragma once

#include <string>
#include <iostream>
#include <unistd.h>
#include <stdlib.h>
#include <fstream>
#include <algorithm>
#include <cstring>
using namespace std;

//constant int for the number of columns the array will hold
const int MAX_COLUMNS = 4;

int globalArray[500][MAX_COLUMNS]; //array used to pass data to consumer functions
int processAmount = 0; //number of processes to be analyzed - user entered

class DATA_GENERATOR
{

  private:
    //the 2D array that holds the process data
    int processArray[500][MAX_COLUMNS];

  public:
    //constructor and destructor
    DATA_GENERATOR();
    ~DATA_GENERATOR();

    //class member function declarations
    void generateData(int processAmount);
    void importData(string filePath, int processAmount);
    void typeData(int processAmount);
    void outputData(int processAmount, int globalArray[][MAX_COLUMNS]);
    void showMenu();
    int getChoice();
    void master();

};
