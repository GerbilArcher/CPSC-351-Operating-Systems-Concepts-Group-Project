//=====================================================================
//CPSC-351-01(10369) pidCoding Group Assignment
//Data Generation Portion - generator.cpp File (File 2 of 2)
//Programmer:     GR
//Group Members:  LG, AL, JM
//Term:           Summer 2021
//=====================================================================

//header file
#include "generator.h"


//************DATA_GENERATOR CLASS MEMBER FUNCTION IMPLEMENTATIONS************
//The DATA_GENERATOR class contains 6 member functions.
//1. Constructor
//2. Destructor
//3. generateData (allows user to have the system generate random data)
//4. importData (allows user to import data from text file to be analyzed)
//5. typeData (allows user to type in data to be analyzed)
//6. outputData (displays the data to be analyzed on the screen)
//7. showMenu (displays the list of menu choices to the user)
//8. getChoice (obtains the user's choice and returns value to master method)
//9. master (the root function called to use all the others in this class)


//----------------------------------------------------------------------------
//beginning of constructor implementation
DATA_GENERATOR::DATA_GENERATOR()
{}
//end of constructor implementation
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
//beginning of destructor implementation
DATA_GENERATOR::~DATA_GENERATOR()
{}
//end of destructor implementation
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
//beginning of generateData method implementation
void DATA_GENERATOR::generateData(int processAmount)
{
  for (int i = 0; i < processAmount; i++) //loop through the rows
  for (int i = 0; i < processAmount; i++) //loop through the rows
  {
    //seed the rand function using system clock time
    srand(time(0));

    //load process ID into 1st column
    processArray[i][0] = (i + 1);

    sleep(1); //sleep for 1 sec to avoid dupe values being generated

    //load burst time (generates random number 1-20) into 2nd column
    processArray[i][1] = rand() % 20 + 1;

    sleep(1); //sleep for 1 sec to avoid dupe values being generated

    //load priority (generates random number 0-10) into 3rd column
    processArray[i][2] = rand() % 11;

    sleep(1); //sleep for 1 sec to avoid dupe values being generated

    //load arrival time (generates random number 0-20) into 4th column
    processArray[i][3] = rand() % 21;
  }
}
//end of generateData method implementation
//----------------------------------------------------------------------------


//----------------------------------------------------------------------------
//beginning of importData method implementation
void DATA_GENERATOR::importData(string filePath, int processAmount)
{
  string buffer = ""; //place to hold imported text for formatting
  char unwantedChars[] = "Pp, "; //characters to be deleted from incoming text

  ifstream file(filePath); //start the input stream for a file we'll call 'file'
  if (file.is_open()) //obligatory check if file is open
  {
    for (int i = 0; i < processAmount; i++) //outer loop for rows
    {
      for (int j = 0; j < MAX_COLUMNS; j++) //inner loop for columns
      {
        file >> buffer; //input text record from file into the buffer string

        for (int p = 0; p < 3; p++) //for loop to remove unwanted characters
        {
          buffer.erase (remove(buffer.begin(), buffer.end(), unwantedChars[p]), buffer.end());
        }

        try
        {
          if (j == 0) //if we're on the first column...
          {
            processArray[i][j] = (i + 1);//...just generate and enter correct pid
          }
          else //if we're on any other column...
          {
            //...convert value from string to int (stoi) and load into processArray
            processArray[i][j] = stoi(buffer);
          }
        }
        catch(...)
        { //if something goes wrong notify the user and indicate what the problem is
          cout << "That didn't work." << endl;
          cout << "Here's your problem:" << buffer << endl << endl;
          sleep(2); //sleep 2 seconds so the user can see the issue
        }

        if (file.eof()) {break;} //break when end of file is reached
      }
    }
    file.close(); //obligatory close the file
  }
}
//end of importData method implementation
//----------------------------------------------------------------------------


//----------------------------------------------------------------------------
//beginning of typeData method implementation
void DATA_GENERATOR::typeData(int processAmount)
{
  system("clear");

  for (int i = 0; i < processAmount; i++) //outer loop for rows
    for (int j = 0; j < MAX_COLUMNS; j++) //inner loop for columns
      {
        if (j == 0) //if we're in the 1st column...
        { //...generate and load correct pid
          processArray[i][j] = (i + 1);
        }
        else if (j == 1) //if we're in the 2nd column...
        { //...user enters burst time for this row...
          cout << "Please enter the burst time for Process " << (i + 1) << ":  ";
          cin >> processArray[i][j]; //...and it's loaded into the array
        }
        else if (j == 2) //if we're in the 3rd column...
        { //...user enters the priority
          cout << "Please enter the priority for Process " << (i + 1) << ":  ";
          cin >> processArray[i][j];  //...and it's loaded into the array
        }
        else if (j == 3) //if we're in the 4th column...
        { //user enters the arrival time
          cout << "Please enter the arrival time for Process " << (i + 1) << ":  ";
          cin >> processArray[i][j];  //...and it's loaded into the array
        }
      }
}
//end of typeData method implementation
//----------------------------------------------------------------------------


//----------------------------------------------------------------------------
//beginning of outputData method implementation
void DATA_GENERATOR::outputData(int processAmount, int globalArray[][MAX_COLUMNS])
{
  system("clear");

  cout << "Here is the data that will be analyzed:" << endl << endl;
  cout << "PID, BT, Priority, AT" << endl;

  for (int i = 0; i < processAmount; i++) //outer loop for rows
  {
    cout << "P"; //put a 'P' in front of each pid int at the beginning of each row
    for (int j = 0; j < MAX_COLUMNS; j++) //inner loop for columns
      {
        globalArray[i][j] = processArray[i][j];
        cout << processArray[i][j]; //output the next value in the row
        if (j != 3) {cout << ", ";} //if it's not the last value in the row
      }                             //output a comma and a space
    cout << endl;
  }
}
//end of outputData method implementation
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
//beginning of showMenu method implementation
void DATA_GENERATOR::showMenu()
{
  //clears the screen and shows the menu options
  //can be recalled whenever the user needs to see the menu since
  //it clears the screen and outputs only
  system("clear");
  cout << "Welcome to the Algorithm Evaluation program." << endl << endl;
  cout << "To kick things off we'll need to know if we're ";
  cout << "going to be generating the \nprocess data to be ";
  cout << "evaluated or if you'll be providing it." << endl << endl;

  cout << "1. Please generate data for me" << endl << endl;
  cout << "2. I will provide the data in a text file" << endl << endl;
  cout << "3. I will type in the data myself" << endl << endl;
  cout << "4. EXIT PROGRAM" << endl << endl << endl;
}
//end of showMenu method implementation
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
//beginning of getChoice method implementation

//accepts user choice and returns the corresponding integer value to the
//function that called it

int DATA_GENERATOR::getChoice()
{
  int userChoice = 0;
  bool isValid = false;

  do
  {
    cout << "Please make your selection and hit Enter: ";
    cin >> userChoice;

    switch(userChoice)
    {
      case 1:
        return 1;

      case 2:
        return 2;

      case 3:
        return 3;

      case 4:
        return 4;

//default case notifies user of invalid selection and ask them to try again
      default:
        cout << endl << endl << "You've made an invalid selection.  ";
        cout << "Please try again." << endl << endl;
//hold (sleep) for 2 seconds before the screen clears so that the user
//case see the invalid selection message
        sleep(2);
        system("clear"); //clear the screen
        this->showMenu(); //show the menu on the screen again
        break;
      }
    } while (!isValid);  //and redo the loop

//note: the loop above should handle all input, so code below is
//just for good measure
    cout << "An error occurred while making the selection.";
    exit(0);

//compiler gives a warning message if int type functions don't
//have return statements at the bottom apparently, so viola:
    return 0;
}
//end of getChoice method implementation
//----------------------------------------------------------------------------


//----------------------------------------------------------------------------
//beginning of master method implementation
void DATA_GENERATOR::master()
{
  string filePath = ""; //filepath for importing data - user entered

  this->showMenu(); //show the user menu of options

  switch(this->getChoice()) //get the user input
  {
    case 1: //user indicates that they want data generated for them
      system("clear");
      cout << "You have selected option 1: Please generate data for me.";
      cout << endl << endl << "How many processes would you like to generate?";

      //user selects the number of processes they want run
      cout << endl << endl << "Number of Processes:  ";
      cin >> processAmount; //get the number of processes from the user

      //generate the data and load it into the 2D array (processArray)
      this->generateData(processAmount);

      //output the data to the screen prove you did it right
      this->outputData(processAmount, globalArray);
      break;

    case 2: //user indicates that they want data imported from a text file
      system("clear");
      cout << "You have selected option 2: I will provide the data in a text file.";
      cout << endl << endl << "Please provide the file path:  ";

      cin >> filePath;  //user inputs the file path

      cout << endl << endl << "How many processes are being imported?";
      cout << endl << endl << "Number of Processes:  ";
      cin >> processAmount; //get the number of processes from the user

      //import the data and put it into the 2D array (processArray)
      this->importData(filePath, processAmount);

      //output the data to the screen prove you did it right
      this->outputData(processAmount,globalArray);
      break;

    case 3:  //user indicates that they want to type in the data themselves
      system("clear");
      cout << "You have selected option 3: I will type in the data myself.";
      cout << endl << endl << "How many processes are being imported?";
      cout << endl << endl << "Number of Processes:  ";
      cin >> processAmount; //get the number of processes from the user

      //accept data from the user and put it into the 2D array (processArray)
      this->typeData(processAmount);

      //output the data to the screen prove you did it right
      this->outputData(processAmount, globalArray);
      break;

    case 4: //user indicates that they wish to exit the program
      cout << endl << endl << "You have selected option 4.  ";
      cout << "Goodbye!" << endl << endl;
      exit(0);
    }

}
//end of master method implementation
//----------------------------------------------------------------------------


//********END OF DATA_GENERATOR CLASS MEMBER FUNCTION IMPLEMENTATIONS*********


//----------------------------------------------------------------------------
//end of program
//----------------------------------------------------------------------------
