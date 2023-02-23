//=====================================================================
//CPSC-351-01(10369) pidCoding Group Assignment
//Sorting Algorithm Portion - algorithms.cpp File (File 2 of 2)
//Programmers:    GR, LG, AL, JM
//Term:           Summer 2021
//=====================================================================

//header file
#include "algorithms.h"
#include<iostream>
#include<fstream>
#include<algorithm>
#include <vector>
#include <cmath>

using namespace std;
ofstream outputFile("output.txt");
//************ALGORITHMS CLASS MEMBER FUNCTION IMPLEMENTATIONS************
//The DATA_GENERATOR class contains X member functions.
//1. Constructor
//2. Destructor
//3. Priority Non-Preemptive Sorting Algorithm
//4. Priority Preemptive Sorting Algorithm
//5. SJF (Shortest Job First) Non-Preemptive Sorting Algorithm
//6. SJF (Shortest Job First) Preemptive Sorting Algorithm
//7. FCFS (First Come First Served) Sorting Algorithm
//8. RR (Round Robin) Sorting Algorithm
//9. TBD


//----------------------------------------------------------------------------
//beginning of constructor implementation
Algorithms::Algorithms()
{}
//end of constructor implementation
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
//beginning of destructor implementation
Algorithms::~Algorithms()
{}
//end of destructor implementation
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
// Threads
void Algorithms::StartInternalThread()
{

  std::thread PriorityPThread(&Algorithms::priorityP, this, globalArray, processAmount);
  std::thread SJFNPThread(&Algorithms::SJFNP, this, globalArray, processAmount);
  std::thread SJFPThread(&Algorithms::SJFP, this, globalArray, processAmount);
  std::thread FCFSThread(&Algorithms::FCFS, this, globalArray, processAmount);
  std::thread RRThread(&Algorithms::RR, this, globalArray, processAmount);
  std::thread PriorityNPThread(&Algorithms::priorityNP, this, globalArray, processAmount);


  PriorityPThread.join();
  SJFPThread.join();
  SJFNPThread.join();
  FCFSThread.join();
  RRThread.join();
  PriorityNPThread.join();
}
// End of THREADS
//----------------------------------------------------------------------------

//beginning of priorityNP method implementation
void Algorithms::priorityNP(int globalArray[][MAX_COLUMNS], int processAmount)
{
    int processes[processAmount][MAX_COLUMNS]; // Declares a new array to store process data

    // Copies data from the global array into the new array
    for (int i = 0; i < processAmount; i++)
    {
        for (int j = 0; j < MAX_COLUMNS; j++)
        {
            processes[i][j] = globalArray[i][j];
        }
    }

    // Create arrays to store process start times, completion times, turnaround times, and wait times
    int st[processAmount];
    int ct[processAmount];
    int tat[processAmount];
    int wt[processAmount];

    // Initializes variables for average waiting time and total waiting time
    double avgWT;
    int totalWT = 0;

    int isCompleted[processAmount]; // Initializes an array to store whether a process is complete
    for (int i = 0; i<processAmount; i++) // Sets all values in the array to 0
    {
        isCompleted[i] = 0;
    }

    int currentTime = 0; // Initializes a timer variable
    int completed = 0; // Initializes a counter of completed processes

    while(completed != processAmount) // Enters a loop that continues until the number of completed processes is equal to the number of processes
    {
        // Initializes an index variable to represent the the processes that meets the criteria to be executed at the current time
        // The variable will change value to a positive number corresponding to an array row value of the process that meets the criteria to be executed at the current time (if there is one)
        int index = -1;
        int min = 1000; // Sets lowest priority value to a high number that will be overwritten
        for(int i = 0; i < processAmount; i++) // Cycles through the processes at the current time
        {
            if(processes[i][3] <= currentTime && isCompleted[i] == 0) // Checks if a process' arrival time is <= the current time and whether it is complete
            {
                if(processes[i][2] < min) // If the process has arrived and is not yet complete, the process' priority is checked versus the current min
                {
                    min = processes[i][2]; // If the current process' priority is lower, it becomes the new min
                    index = i; // Index takes on the array row value of the process in question, indicating the process meets the criteria to be executed at the current time
                }
                else if(processes[i][2] == min) // If the current process' priority equals the current minimum priority...
                {
                    if(processes[i][3] < processes[index][3]) // ...Check the arrival time of the current process vs the other process that currently occupies the min position
                    {
                        min = processes[i][2]; // If the current process has an earlier arrival time, its priority becomes the new min
                        index = i; // Index takes on the array row value of the process in question, indicating the process meets the criteria to be executed at the current time
                    }
                    else if(processes[i][3] == processes[index][3]) // If both the priority and arrival time of the two processes with matching priorities are the same, check the PID of the processes
                    {
                        if(processes[i][0] < processes[index][0])
                        {
                            min = processes[i][2]; // If the current process has an earlier PID, its priority becomes the new min
                            index = i; // Index takes on the array row value of the process in question, indicating the process meets the criteria to be executed at the current time
                        }
                    }
                }
            }
        }

        // Checks whether index is not -1, indicating a process meets the criteria to execute at the current time
        // If so, the process is executed
        if(index != -1)
        {
            st[index] = currentTime; // Sets the start time of the process to the current time
            ct[index] = st[index] + processes[index][1]; // Sets the completion time of the process to the start time of the process plus its burst time
            tat[index] = ct[index] - processes[index][3]; // Calculates the turnaround time of the process by subtracting the process' arrival time from its completion time
            wt[index] = tat[index] - processes[index][1]; // Calculates the wait time of the process by subtracting the process' burst time from its turnaround time
            totalWT += wt[index]; // Adds the wait time of the process that executed to the total wait time
            isCompleted[index] = 1; // Indicates that the process is now completed
            completed++; // Increments the count of completed processes
            currentTime = ct[index];  // Current time jumps to the completion time of the process that just completed
        }
        // If no processes met the criteria to execute at the current time, the time increments and the loop checks again
        else
        {
            currentTime++;
        }

    }

    avgWT = (double) totalWT / processAmount; // Calculates the average wait time, converting it to a decimal
    PriorityNPAvgWT = avgWT;
    /*cout<<"PID\t"<<"BT\t"<<"Priority\t"<<"AT\t"<<"Start Time\t"<<"Completion Time\t"<<"TAT\t"<<"WT\t"<<endl; // Outputs table headers
    // Outputs data values for each process
    for(int i = 0; i < processAmount; i++)
    {
        cout<<processes[i][0]<<"\t"<<processes[i][1]<<"\t"<<processes[i][2]<<"\t\t\t"<<processes[i][3]
        <<"\t"<<st[i]<<"\t\t\t"<<ct[i]<<"\t\t\t\t"<<tat[i]<<"\t"<<wt[i]<<"\t"<<endl;
    }
    cout<<"Average Wait Time = "<<avgWT<<endl<<endl; // Outputs the average wait time*/

    mtx.lock();
    outputFile << "\nAverage waiting time Priority Non-Preemptive = "<< avgWT << endl;
    mtx.unlock();
}
//end of priorityNP method implementation
//----------------------------------------------------------------------------


//----------------------------------------------------------------------------
//beginning of priorityP method implementation
void Algorithms::priorityP(int globalArray[][MAX_COLUMNS], int processAmount)
{
    int processes[processAmount][MAX_COLUMNS]; // Declares a new array to store process data

    // Copies data from the global array into the new array
    for (int i = 0; i < processAmount; i++)
    {
        for (int j = 0; j < MAX_COLUMNS; j++)
        {
            processes[i][j] = globalArray[i][j];
        }
    }

    // Create arrays to store process start times, completion times, turnaround times, and wait times
    int st[processAmount];
    int ct[processAmount];
    int tat[processAmount];
    int wt[processAmount];

    int newBT[processAmount]; // Creates an array to store the new burst times of each process after each time interval

    // Fills the array with the original burst times
    for (int i=0; i< processAmount; i++)
    {
        newBT[i] = processes[i][1];
    }

    // Initializes variables for average waiting time and total waiting time
    double avgWT;
    int totalWT = 0;

    int isCompleted[processAmount]; // Initializes an array to store whether a process is complete

    // Sets all values in the array to 0
    for (int i = 0; i<processAmount; i++)
    {
        isCompleted[i] = 0;
    }

    int currentTime = 0; // Initializes a timer variable
    int completed = 0; // Initializes a counter of completed processes

    while(completed != processAmount) // Enters a loop that continues until the number of completed processes is equal to the number of processes
    {
        // Initializes an index variable to represent the the processes that meets the criteria to be executed at the current time
        // The variable will change value to a positive number corresponding to an array row value of the process that meets the criteria to be executed at the current time (if there is one)
        int index = -1;
        int min = 1000; // Sets lowest priority value to a high number that will be overwritten
        for(int i = 0; i < processAmount; i++) // Cycles through the processes at the current time
        {
            if(processes[i][3] <= currentTime && isCompleted[i] == 0) // Checks if a process' arrival time is <= the current time and whether it is complete
            {
                if(processes[i][2] < min) // If the process has arrived and is not yet complete, the process' priority is checked versus the current min
                {
                    min = processes[i][2]; // If the current process' priority is lower, it becomes the new min
                    index = i; // Index takes on the array row value of the process in question, indicating the process meets the criteria to be executed at the current time
                }
                else if(processes[i][2] == min) // If the current process' priority equals the current minimum priority...
                {
                    if(processes[i][3] < processes[index][3]) // ...Check the arrival time of the current process vs the other process with the same priority
                    {
                        min = processes[i][2]; // If the current process has an earlier arrival time, its priority becomes the new min
                        index = i; // Index takes on the array row value of the process in question, indicating the process meets the criteria to be executed at the current time
                    }
                    else if(processes[i][3] == processes[index][3]) // If both the priority and arrival time of the two processes with matching priorities are the same, check the PID of the processes
                    {
                        if(processes[i][0] < processes[index][0])
                        {
                            min = processes[i][2]; // If the current process has an earlier PID, its priority becomes the new min
                            index = i; // Index takes on the array row value of the process in question, indicating the process meets the criteria to be executed at the current time
                        }
                    }
                }
            }
        }

        // Checks whether index is not -1, indicating a process meets the criteria to execute at the current time
        // If so, the process is executed
        if(index != -1)
        {
            // Checks whether the burst time of the process currently is the same as its original burst time
            // This is how to check if a process has already started previously
            if (newBT[index] == processes[index][1])
            {
                st[index] = currentTime; // If the process' current burst time is the same as its original burst time (meaning it had not started previously), set the process' start time to the current time
            }

            newBT[index]--; // Decrement the current process' burst time
            currentTime++; // Increment the current time

            if (newBT[index] == 0) // If the process finished during the previous time interval...
            {
                isCompleted[index] = 1; // ...Mark it as completed in the completion array
                completed++; // Increment the number of completed processes
                ct[index] = currentTime; // Note the completion time as the current time after it was incremented
                tat[index] = ct[index] - processes[index][3]; // Calculate TAT (CT-AT)
                wt[index] = tat[index] - processes[index][1]; // Calculate WT (TAT-BT). Make sure to use the original BT value in the calculation as the new BT will be 0
                totalWT += wt[index]; // Sum the process' WT to the total WT
            }
        }
        // If no processes met the criteria to execute at the current time, the time increments and the loop checks again
        else
        {
            currentTime++;
        }

    }

    avgWT = (double) totalWT / processAmount; // Calculates the average wait time, converting it to a decimal
    PriorityPAvgWT = avgWT;
    /*cout<<"PID\t"<<"BT\t"<<"Priority\t"<<"AT\t"<<"Start Time\t"<<"Completion Time\t"<<"TAT\t"<<"WT\t"<<endl; // Outputs table headers
    // Outputs data values for each process
    for(int i = 0; i < processAmount; i++)
    {
        cout<<processes[i][0]<<"\t"<<processes[i][1]<<"\t"<<processes[i][2]<<"\t\t\t"<<processes[i][3]
        <<"\t"<<st[i]<<"\t\t\t"<<ct[i]<<"\t\t\t\t"<<tat[i]<<"\t"<<wt[i]<<"\t"<<endl;
    }
    cout<<"Average Wait Time = "<<avgWT<<endl<<endl; // Outputs the average wait time*/

    mtx.lock();
    outputFile << "\nAverage waiting time Priority Preemptive = "<< avgWT << endl;
    mtx.unlock();
}
//end of priorityP method implementation
//----------------------------------------------------------------------------


//----------------------------------------------------------------------------
//beginning of SJFNP method implementation
void Algorithms::SJFNP(int globalArray[][MAX_COLUMNS], int processAmount)
{
    int processes[processAmount][MAX_COLUMNS]; // Declares a new array to store process data

    // Copies data from the global array into the new array
    for (int i = 0; i < processAmount; i++)
    {
        for (int j = 0; j < MAX_COLUMNS; j++)
        {
            processes[i][j] = globalArray[i][j];
        }
    }

    // Create arrays to store process start times, completion times, turnaround times, and wait times
    int st[processAmount];
    int ct[processAmount];
    int tat[processAmount];
    int wt[processAmount];

    // Initializes variables for average waiting time and total waiting time
    double avgWT;
    int totalWT = 0;

    int isCompleted[processAmount]; // Initializes an array to store whether a process is complete
    for (int i = 0; i<processAmount; i++) // Sets all values in the array to 0
    {
        isCompleted[i] = 0;
    }

    int currentTime = 0; // Initializes a timer variable
    int completed = 0; // Initializes a counter of completed processes

    while(completed != processAmount) // Enters a loop that continues until the number of completed processes is equal to the number of processes
    {
        // Initializes an index variable to represent the the processes that meets the criteria to be executed at the current time
        // The variable will change value to a positive number corresponding to an array row value of the process that meets the criteria to be executed at the current time (if there is one)
        int index = -1;
        int min = 1000; // Sets lowest BT value to a high number that will be overwritten
        for(int i = 0; i < processAmount; i++) // Cycles through the processes at the current time
        {
            if(processes[i][3] <= currentTime && isCompleted[i] == 0) // Checks if a process' arrival time is <= the current time and whether it is complete
            {
                if(processes[i][1] < min) // If the process has arrived and is not yet complete, the process' BT is checked versus the current min BT
                {
                    min = processes[i][1]; // If the current process' BT is lower, it becomes the new min
                    index = i; // Index takes on the array row value of the process in question, indicating the process meets the criteria to be executed at the current time
                }
                else if(processes[i][1] == min) // If the current process' BT equals the current minimum BT...
                {
                    if(processes[i][3] < processes[index][3]) // ...Check the arrival time of the current process vs the other process that currently occupies the min position
                    {
                        min = processes[i][1]; // If the current process has an earlier arrival time, its BT becomes the new min
                        index = i; // Index takes on the array row value of the process in question, indicating the process meets the criteria to be executed at the current time
                    }
                    else if(processes[i][3] == processes[index][3]) // If both the BT and arrival time of the two processes with matching priorities are the same, check the PID of the processes
                    {
                        if(processes[i][0] < processes[index][0])
                        {
                            min = processes[i][1]; // If the current process has an earlier PID, its BT becomes the new min
                            index = i; // Index takes on the array row value of the process in question, indicating the process meets the criteria to be executed at the current time
                        }
                    }
                }
            }
        }

        // Checks whether index is not -1, indicating a process meets the criteria to execute at the current time
        // If so, the process is executed
        if(index != -1)
        {
            st[index] = currentTime; // Sets the start time of the process to the current time
            ct[index] = st[index] + processes[index][1]; // Sets the completion time of the process to the start time of the process plus its burst time
            tat[index] = ct[index] - processes[index][3]; // Calculates the turnaround time of the process by subtracting the process' arrival time from its completion time
            wt[index] = tat[index] - processes[index][1]; // Calculates the wait time of the process by subtracting the process' burst time from its turnaround time
            totalWT += wt[index]; // Adds the wait time of the process that executed to the total wait time
            isCompleted[index] = 1; // Indicates that the process is now completed
            completed++; // Increments the count of completed processes
            currentTime = ct[index];  // Current time jumps to the completion time of the process that just completed
        }
        // If no processes met the criteria to execute at the current time, the time increments and the loop checks again
        else
        {
            currentTime++;
        }

    }

    avgWT = (double) totalWT / processAmount; // Calculates the average wait time, converting it to a decimal

    /*cout<<"PID\t"<<"BT\t"<<"Priority\t"<<"AT\t"<<"Start Time\t"<<"Completion Time\t"<<"TAT\t"<<"WT\t"<<endl; // Outputs table headers
    // Outputs data values for each process
    for(int i = 0; i < processAmount; i++)
    {
        cout<<processes[i][0]<<"\t"<<processes[i][1]<<"\t"<<processes[i][2]<<"\t\t\t"<<processes[i][3]
        <<"\t"<<st[i]<<"\t\t\t"<<ct[i]<<"\t\t\t\t"<<tat[i]<<"\t"<<wt[i]<<"\t"<<endl;
    }
    cout<<"Average Wait Time = "<<avgWT<<endl<<endl; // Outputs the average wait time*/

  // Writes the average wait time to a global variable
    SJFNPAvgWT = avgWT;

  // Writes to the output file
    mtx.lock();
    outputFile << "\nAverage waiting time SJF Non-Preemptive = "<< avgWT << endl;
    mtx.unlock();
}
//end of SJFNP method implementation
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
//beginning of SJFP method implementation
void Algorithms::SJFP(int globalArray[][MAX_COLUMNS], int processAmount)
{
  int processes[processAmount][MAX_COLUMNS]; // Declares a new array to store process data

  // Copies data from the global array into the new array
  for (int i = 0; i < processAmount; i++)
  {
    for (int j = 0; j < MAX_COLUMNS; j++)
    {
      processes[i][j] = globalArray[i][j];
    }
  }

  // Create arrays to store process start times, completion times, turnaround times, and wait times
  int st[processAmount];
  int ct[processAmount];
  int tat[processAmount];
  int wt[processAmount];

  int newBT[processAmount]; // Creates an array to store the new burst times of each process after each time interval

  // Fills the array with the original burst times
  for (int i=0; i< processAmount; i++)
  {
    newBT[i] = processes[i][1];
  }

  // Initializes variables for average waiting time and total waiting time
  double avgWT;
  int totalWT = 0;

  int isCompleted[processAmount]; // Initializes an array to store whether a process is complete
  // Sets all values in the array to 0
  for (int i = 0; i<processAmount; i++)
  {
    isCompleted[i] = 0;
  }

  int currentTime = 0; // Initializes a timer variable
  int completed = 0; // Initializes a counter of completed processes

  while(completed != processAmount) // Enters a loop that continues until the number of completed processes is equal to the number of processes
  {
    // Initializes an index variable to represent the the processes that meets the criteria to be executed at the current time
    // The variable will change value to a positive number corresponding to an array row value of the process that meets the criteria to be executed at the current time (if there is one)
    int index = -1;
    int min = 1000; // Sets lowest BT value to an artificially high number that will be overwritten
    for(int i = 0; i < processAmount; i++) // Cycles through the processes at the current time
    {
      if(processes[i][3] <= currentTime && isCompleted[i] == 0) // Checks if a process' arrival time is <= the current time and whether it is complete
      {
        if(processes[i][1] < min) // If the process has arrived and is not yet complete, the process' BT is checked versus the current min BT
        {
          min = processes[i][1]; // If the current process' BT is lower, it becomes the new min
          index = i; // Index takes on the array row value of the process in question, indicating the process meets the criteria to be executed at the current time
        }
        else if(processes[i][1] == min) // If the current process' BT equals the current minimum BT...
        {
          if(processes[i][3] < processes[index][3]) // ...Check the arrival time of the current process vs the other process with the same burst time
          {
            min = processes[i][1]; // If the current process has an earlier arrival time, its BT becomes the new min
            index = i; // Index takes on the array row value of the process in question, indicating the process meets the criteria to be executed at the current time
          }
          else if(processes[i][3] == processes[index][3]) // If both the burst time and arrival time of the two processes with matching priorities are the same, check the PID of the processes
          {
            if(processes[i][0] < processes[index][0])
            {
              min = processes[i][1]; // If the current process has an earlier PID, its burst time becomes the new min
              index = i; // Index takes on the array row value of the process in question, indicating the process meets the criteria to be executed at the current time
            }
          }
        }
      }
    }

    // Checks whether index is not -1, indicating a process meets the criteria to execute at the current time
    // If so, the process is executed
    if(index != -1)
    {
      // Checks whether the burst time of the process currently is the same as its original burst time
      // This is how to check if a process has already started previously
      if (newBT[index] == processes[index][1])
      {
        st[index] = currentTime; // If the process' current burst time is the same as its original burst time (meaning it had not started previously), set the process' start time to the current time
      }
      newBT[index]--; // Decrement the current process' burst time
      currentTime++; // Increment the current time

      if (newBT[index] == 0) // If the process finished during the previous time interval...
      {
        isCompleted[index] = 1; // ...Mark it as completed in the completion array
        completed++; // Increment the number of completed processes
        ct[index] = currentTime; // Note the completion time as the current time after it was incremented
        tat[index] = ct[index] - processes[index][3]; // Calculate TAT (CT-AT)
        wt[index] = tat[index] - processes[index][1]; // Calculate WT (TAT-BT). Make sure to use the original BT value in the calculation as the new BT will be 0
        totalWT += wt[index]; // Sum the process' WT to the total WT
      }
    }

    // If no processes met the criteria to execute at the current time, the time increments and the loop checks again
    else
    {
      currentTime++;
    }
  }

  avgWT = (double) totalWT / processAmount; // Calculates the average wait time, converting it to a decimal

  /*cout<<"PID\t"<<"BT\t"<<"Priority\t"<<"AT\t"<<"Start Time\t"<<"Completion Time\t"<<"TAT\t"<<"WT\t"<<endl; // Outputs table headers
  // Outputs data values for each process
  for(int i = 0; i < processAmount; i++)
  {
    cout<<processes[i][0]<<"\t"<<processes[i][1]<<"\t"<<processes[i][2]<<"\t\t\t"<<processes[i][3]<<"\t"<<st[i]<<"\t\t\t"<<ct[i]<<"\t\t\t\t"<<tat[i]<<"\t"<<wt[i]<<"\t"<<endl;
  }
  cout<<"Average Wait Time = "<<avgWT<<endl<<endl; // Outputs the average wait time*/

  // Writes the wait time to a global variable
  SJFPAvgWT = avgWT;

  // Writes to the output file
  mtx.lock();
  outputFile << "\nAverage waiting time SJF Preemptive = "<< avgWT << endl;
  mtx.unlock();
}

//end of SJFP method implementation
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
//beginning of FCFS method implementation
//Comparator

void Algorithms::FCFS(int globalArray[][MAX_COLUMNS], int processAmount)
{
  int processes[processAmount][MAX_COLUMNS]; // Declares a new array to store process data

  // Copies data from the global array into the new array
  for (int i = 0; i < processAmount; i++)
  {
      for (int j = 0; j < MAX_COLUMNS; j++)
      {
          processes[i][j] = globalArray[i][j];
      }
  }

  // Create arrays to store process start times, completion times, turnaround times, and wait times
  int st[processAmount];
  int ct[processAmount];
  int tat[processAmount];
  int wt[processAmount];

  // Initializes variables for average waiting time and total waiting time
  double avgWT;
  int totalWT = 0;

  int isCompleted[processAmount]; // Initializes an array to store whether a process is complete
  for (int i = 0; i<processAmount; i++) // Sets all values in the array to 0
  {
      isCompleted[i] = 0;
  }

  int currentTime = 0; // Initializes a timer variable
  int completed = 0; // Initializes a counter of completed processes

  while(completed != processAmount) // Enters a loop that continues until the number of completed processes is equal to the number of processes
  {
      // Initializes an index variable to represent the the processes that meets the criteria to be executed at the current time
      // The variable will change value to a positive number corresponding to an array row value of the process that meets the criteria to be executed at the current time (if there is one)
      int index = -1;
      int min = 1000; // Sets lowest BT value to a high number that will be overwritten
      for(int i = 0; i < processAmount; i++) // Cycles through the processes at the current time
      {
          if(processes[i][3] <= currentTime && isCompleted[i] == 0) // Checks if a process' arrival time is <= the current time and whether it is complete
          {
              if(processes[i][3] < min) // If the process has arrived and is not yet complete, the process' BT is checked versus the current min BT
              {
                  min = processes[i][3]; // If the current process' BT is lower, it becomes the new min
                  index = i; // Index takes on the array row value of the process in question, indicating the process meets the criteria to be executed at the current time
              }
              else if(processes[i][3] == min) // If the current process' BT equals the current minimum BT...
              {
                  if(processes[i][0] < processes[index][0]) // ...Check the arrival time of the current process vs the other process that currently occupies the min position
                  {
                      min = processes[i][1]; // If the current process has an earlier arrival time, its BT becomes the new min
                      index = i; // Index takes on the array row value of the process in question, indicating the process meets the criteria to be executed at the current time
                  }
              }
          }
      }

      if(index != -1)
      {
          st[index] = currentTime; // Sets the start time of the process to the current time
          ct[index] = st[index] + processes[index][1]; // Sets the completion time of the process to the start time of the process plus its burst time
          tat[index] = ct[index] - processes[index][3]; // Calculates the turnaround time of the process by subtracting the process' arrival time from its completion time
          wt[index] = tat[index] - processes[index][1]; // Calculates the wait time of the process by subtracting the process' burst time from its turnaround time
          totalWT += wt[index]; // Adds the wait time of the process that executed to the total wait time
          isCompleted[index] = 1; // Indicates that the process is now completed
          completed++; // Increments the count of completed processes
          currentTime = ct[index];  // Current time jumps to the completion time of the process that just completed
      }
      // If no processes met the criteria to execute at the current time, the time increments and the loop checks again
      else
      {
          currentTime++;
      }


    }

  avgWT = (double) totalWT / processAmount;

  FCFSAvgWT = avgWT;

  outputFile << "\nAverage waiting time FCFS = "<< avgWT << endl;

}
//end of FCFS method implementation
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
//beginning of RR method implementation

//end of getProcesses method implementation
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
//beginning of getProcesses method implementation

//end of getProcesses method implementation
//----------------------------------------------------------------------------

//end of RR method implementation
//----------------------------------------------------------------------------


//----------------------------------------------------------------------------
//beginning of RR (New) method implementation
void Algorithms::RR(int globalArray[][MAX_COLUMNS], int processAmount)
{
    int processes[processAmount][MAX_COLUMNS]; // Declares a new array to store process data

    // Copies data from the global array into the new array
    for (int i = 0; i < processAmount; i++)
    {
        for (int j = 0; j < MAX_COLUMNS; j++)
        {
            processes[i][j] = globalArray[i][j];
        }
    }

    // Create arrays to store process start times, completion times, turnaround times, and wait times
    int st[processAmount];
    int ct[processAmount];
    int tat[processAmount];
    int wt[processAmount];

    int newBT[processAmount]; // Creates an array to store the new burst times of each process after each time interval

    // Fills the array with the original burst times
    for (int i=0; i< processAmount; i++)
    {
        newBT[i] = processes[i][1];
    }

    int newAT[processAmount]; // Creates a new array to store arrival times of each process separately

    // Fills the array with the original arrival times
    for (int i=0; i< processAmount; i++)
    {
        newAT[i] = processes[i][3];
    }

    // Initializes variables for average waiting time and total waiting time
    double avgWT;
    int totalWT = 0;

    int isCompleted[processAmount]; // Initializes an array to store whether a process is complete

    // Sets all values in the array to 0
    for (int i = 0; i<processAmount; i++)
    {
        isCompleted[i] = 0;
    }

    int currentTime = 0; // Initializes a timer variable
    int completed = 0; // Initializes a counter of completed processes

    while(completed != processAmount) // Enters a loop that continues until the number of completed processes is equal to the number of processes
    {
        for(int i = 0; i < processAmount; i++) // Cycles through the processes at the current time
        {
          int minAT = 1000; // Resets the minimum AT to an artifically high number that will be overwritten

          // Finds the lowest AT among the processes
          for (int i = 0; i<processAmount; i++)
          {
            if (newAT[i] < minAT)
              minAT = newAT[i];
          }

          // If no processes have arrived at the current time, increment the time
            if(minAT > currentTime)
            {
              currentTime++;
            }

            if(processes[i][3] <= currentTime && isCompleted[i] == 0) // Checks if a process' arrival time is <= the current time and whether it is complete
            {
              if (newBT[i] == processes[i][1])
              {
                  st[i] = currentTime; // If the process' current burst time is the same as its original burst time (meaning it had not started previously), set the process' start time to the current time
              }

              newBT[i]--; // Decrement the current process' burst time
              currentTime++; // Increment the current time

              if (newBT[i] == 0) // If the process finished during the previous time interval
              {
                  newAT[i]=1001; // Set that process' AT to an artificially high number so it no longer gets calculated when finding the lowest AT
                  isCompleted[i] = 1; // Mark the process as completed in the completion array
                  completed++; // Increment the number of completed processes
                  ct[i] = currentTime; // Note the completion time as the current time after it was incremented
                  tat[i] = ct[i] - processes[i][3]; // Calculate TAT (CT-AT)
                  wt[i] = tat[i] - processes[i][1]; // Calculate WT (TAT-BT). Make sure to use the original BT value in the calculation as the new BT will be 0
                  totalWT += wt[i]; // Sum the process' WT to the total WT
              }
            }
            else
            {
                continue; // Run the loop again until all processes have completed
            }
        }
    }

    avgWT = (double) totalWT / processAmount; // Calculates the average wait time, converting it to a decimal

    /*cout<<"PID\t"<<"BT\t"<<"Priority\t"<<"AT\t"<<"Start Time\t"<<"Completion Time\t"<<"TAT\t"<<"WT\t"<<endl; // Outputs table headers
    // Outputs data values for each process
    for(int i = 0; i < processAmount; i++)
    {
        cout<<processes[i][0]<<"\t"<<processes[i][1]<<"\t"<<processes[i][2]<<"\t\t\t"<<processes[i][3]
        <<"\t"<<st[i]<<"\t\t\t"<<ct[i]<<"\t\t\t\t"<<tat[i]<<"\t"<<wt[i]<<"\t"<<endl;
    }
    cout<<"Average Wait Time = "<<avgWT<<endl<<endl; // Outputs the average wait time*/

    RRAvgWT = avgWT; // Save the average wait time to a global variable

    // Write the average waiting time to the output file
    mtx.lock();
    outputFile << "\nAverage waiting time Round Robin = "<< avgWT << endl;
    mtx.unlock();
}
//end of RR (New) method implementation
//----------------------------------------------------------------------------


//----------------------------------------------------------------------------
//beginning of calcMinWT method implementation
void Algorithms::calcMinAvgWT()
{
  mtx.lock();
  // Calculate the lowest average wait time among the scheduling algorithms and save to a variable
  double minimums[6] = { PriorityNPAvgWT, PriorityPAvgWT, SJFPAvgWT, RRAvgWT, FCFSAvgWT, SJFNPAvgWT };
  double minAvgWT = 1000;
  for (int i = 0; i < 6; i++)
  {
    if (minimums[i] < minAvgWT)
    {
      minAvgWT = minimums[i];
    }
  }

  string displayMsg;

  // Determine the contents of the display message string based on which scheduling algorithm generated the lowest average WT
  if (minAvgWT == PriorityNPAvgWT)
    displayMsg = "Thus, the Priority Non-Preemptive policy results in the minimum average waiting time\n";
  else if (minAvgWT == PriorityPAvgWT)
    displayMsg = "Thus, the Priority Preemptive policy results in the minimum average waiting time\n";
  else if (minAvgWT == SJFPAvgWT)
    displayMsg = "Thus, the SJF Preemptive policy results in the minimum average waiting time\n";
  else if (minAvgWT == RRAvgWT)
    displayMsg = "Thus, the Round Robin policy results in the minimum average waiting time\n";
  else if (minAvgWT == FCFSAvgWT)
    displayMsg = "Thus, the First Come First Server policy results in the minimum average waiting time\n";
  else if (minAvgWT == SJFNPAvgWT)
    displayMsg = "Thus, the SJF Non Preemptive policy results in the minimum average waiting time\n";
  // Write the string saying which scheduling algorithm produced the lowest average WT

  outputFile << displayMsg;
  mtx.unlock();
}
//end of RR calcMinWT method implementation
//----------------------------------------------------------------------------


//********END OF ALGORITHMS CLASS MEMBER FUNCTION IMPLEMENTATIONS*********


//----------------------------------------------------------------------------
//end of program
//----------------------------------------------------------------------------
