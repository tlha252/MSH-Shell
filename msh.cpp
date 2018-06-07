#include <iostream>
#include <string>
#include <sstream>
#include<stdlib.h>
#include<vector>
#include<cstdio>
#include<unistd.h>
#include<stdio.h>
#include<sys/types.h>
#include<sys/wait.h>
#include <algorithm>
#include <map>      // map
#include <queue>
#include<fstream>

using namespace std;

map <string,string> vars;//a map to hold variables created by setvar
map <pid_t,string> procs;//map to match pid's to their processes
queue <string> completes;//queue to hold all of the completed back ground processes. after user input, they will be printed then popped.

#define TEMP_FILE "buffer.txt"

void setvar(vector<string> input, int inputSize)
{
  string value;
  switch(inputSize){
      case 1 :
          cout << "Expected 3 tokens got " << inputSize << endl; //if the number of parameters is incorrect
          break;
      case 3 :
      {
          value = input[2]; //sets the string named value equal to the 2nd token in input
          if((value[0] == '"') && (value[value.length()-1] == '"') )//if it starts and ends with quotations, it will remove them before putting it in a variable
            value = value.substr(1,value.length()-2);//removes the quotations
            vars[ input[1] ] = value;//puts the variable in a map called vars, where the variable name is the key and the value is the value
          break;
      }
      default :
          cout << "Expected 3 tokens got " << inputSize << endl;// if there is an incorrect number of parameters
          break;
  }

}

void setprompt(string &shellPrompt, vector<string>userInput, int inputSize)
{
    switch(inputSize)
    {
        case 1 : // not enough parameters
            cout << "Expected 2 tokens got " << inputSize << endl;
            break;
        case 2 : // proper parameters
            shellPrompt = userInput[1]; // reassigns the passed value of shellPrompt to the second element of userInput
            break;
        default : // too many parameters
            cout << "Expected 2 tokens got " << inputSize << endl;
            break;
    }
}
void setdir(vector<string>userInput, int inputSize)
{
  int success;
    switch(inputSize)
    {
      case 1: //not enough parameters
          cout << "Expected 2 tokens got " << inputSize << endl;
          break;
      case 2:
          {
            const char* PATH = ((char*)userInput[1].c_str());
            success = chdir(PATH);
          //  cout<<"wd: "<<getwd()<<endl;
            if(success < 0) // if it fails print to standard error
              perror("chdir");
            break;
          }
      default : // too many parameters
          cout << "Expected 2 tokens got " << inputSize << endl;
          break;
    }
}

void showprocs(int inputSize)
{
  switch(inputSize)
  {
    case 1 : // if the size of the input vector is one, execute the code
    {
      map<int,string>::iterator it; // iterator to parse through the map
      cout <<"The current running background processes are: " << endl;
        for (it=procs.begin(); it!=procs.end(); it++) //iterates through the map "procs" to print each process currently being run
            cout << "     " <<it->second<<endl;//prints each value in the map, no keys(the keys are pid, don't need to be shown)
      break;
    }
    default : // otherwise the improper amount of parameters have been passed
    cout << "Expected 1 token got " << inputSize << endl;
    break;
  }
}

void done(int &returnStatus, int &runShell, vector<string>userInput, int inputSize)
{
        switch(inputSize)
        {
            case 1 : // if the size of the input vector is one, it implies that the user only input done, so runShell can increment to exit the program
                runShell++;
                break;
            case 2 :
                returnStatus = atoi(userInput[1].c_str()); // if the size of the input vector is two, it implies that the user input done and a parameter for the return value.
                runShell++; // sets returnStatus equal to the integer conversion of the [1] element of the input vector and increments runShell to exit program
                 break;
            default : // default if the input is greater than 3 for whatever reason do this. Do not know if this is correct so talk to Connor about it
                cout << "too many parameters to execute done" << endl;
                break;
        }
}

void run(vector<string>userInput, int inputSize)
{
  // --------------------------START OF CONVERSION FROM VEC<STRING> TO CHAR**---------------------------------
    vector<char *> commandVec;
    if(inputSize == 1)//won't run if the only token is "run"
    {
      cout<<"Too few parameters, need more than one."<<endl;
      return;
    }

    for (int i = 1; i < inputSize; i++)
    {
        commandVec.push_back((char*)userInput[i].c_str());
    }
    commandVec.push_back(NULL);

    char** command = &commandVec[0];
    // --------------------------END OF CONVERSION FROM VEC<STRING> TO CHAR**---------------------------------

    pid_t pid; // Process ID declaration of type pid_t
    int status; // status integer for waitpid to return specfic values

    pid = fork();  // forks() splits the shell and makes a copy of it. The child will execute the passed commands and the parent will be the shell.
    if (pid == 0)
    {  // Child process runs here
        if (execvp(command[0],command) == -1) // if execvp() fails
        {
            perror("msh"); // prints exec error to standard output
        }
        exit(EXIT_FAILURE); // exit with EXIT_FAILURE after printing error
    }
    else if (pid < 0)
    {    // prints msh error if there is an issue with forking
        perror("msh");
    }
    else
    {    //Parent process runs here
        do{
            waitpid(pid, &status, WUNTRACED);
        }while (!WIFEXITED(status) && !WIFSIGNALED(status));
    }
  }

void fly(vector<string>userInput, int inputSize)
{
  // --------------------------START OF CONVERSION FROM VEC<STRING> TO CHAR**---------------------------------
    vector<char *> commandVec;
    if(inputSize == 1)//won't run if the only token is "fly"
    {
      cout<<"Too few parameters, need more than one."<<endl;
      return;
    }
    for (int i = 1; i < inputSize; i++)
    {
        commandVec.push_back((char*)userInput[i].c_str());
    }
    commandVec.push_back(NULL);

    char** command = &commandVec[0];
    // --------------------------END OF CONVERSION FROM VEC<STRING> TO CHAR**---------------------------------

    pid_t pid; // Process ID declaration of type pid_t
    int status; // status integer for waitpid to return specfic values

    pid = fork(); // forks() splits the shell and makes a copy of it. The child will execute the passed commands and the parent will be the shell.
    if (pid == 0)
    { // Child process runs here
        if (execvp(command[0],command) == -1) // if execvp does not execute the passed command correctly
        {
            perror("execvp"); // prints standard exec error to output
        }
        exit(EXIT_FAILURE); // exit with EXIT_FAILURE after printing error message to standard output
    }
    else if (pid < 0)
    {
        perror("fork");  // prints msh error if there is an issue with forking
    }
     else
    {
        //Parent process runs here
        procs[pid] = command[0];//adds the key pid and the value (the function name) to procs
                waitpid(pid, &status, WNOHANG); //waitpid returns the status of the process ID passed to it (pid of the child) and stores it in returnStatus
                // The WNOHANG Macro Parameter tells waitpid() to have the parent process not wait for the child to terminate before executing itself
    }
}

void tovar(vector<string> userInput, int inputSize)//executes the function passed in
{
  vector<char *> commandVec;
  if(inputSize == 1)//won't run if the only token is "tovar"
  {
    cout<<"Too few parameters, need more than one."<<endl;
    return;
  }

  string varname = userInput[1]; // variable to store desired variable name

  for (int i = 2; i < inputSize; i++)
  {
      commandVec.push_back((char*)userInput[i].c_str());
  }
  commandVec.push_back(NULL);

  char** command = &commandVec[0];
  // --------------------------END OF CONVERSION FROM VEC<STRING> TO CHAR**---------------------------------



  pid_t pid; // Process ID declaration of type pid_t
  int status; // status integer for waitpid to return specfic valuesTEMP_FILE,"w",stdout
;
  pid = fork();  // forks() splits the shell and makes a copy of it. The child will execute the passed commands and the parent will be the shell.
  if (pid == 0)
  {  // Child process runs here
    freopen(TEMP_FILE,"w",stdout);
      if (execvp(command[0],command) == -1) // if execvp() fails
      {
          perror("msh"); // prints exec error to standard output
      }
      exit(EXIT_FAILURE); // exit with EXIT_FAILURE after printing error
  }
  else if (pid < 0)
  {    // prints msh error if there is an issue with forking
      perror("msh");
  }
  else
  {    //Parent process runs here
          (void)waitpid(pid, &status, 0);
          ifstream iftemp(TEMP_FILE);
          stringstream buff;
          buff << iftemp.rdbuf();
          vars[varname] = buff.str();
  }

}

void handler(int sig) //handler that is referenced by the signal() call in main(). Used to identify that background processes have ended
{
    pid_t pid; //defines pid to represent the child process
    map<int,string>::iterator it; // iterator to loop through map of processes

    pid = wait(NULL); // waits until processes have executed

    string old_proc = procs[pid];//holds the process to be removed in a string
    it = procs.find(pid); //finds pid's position in the map
    procs.erase(it); //removes pid from the map
    if(old_proc != "\0")//only add to queue if the string isn't empty
      completes.push("Completed: "+old_proc+"\n"); //pushes the completed process onto the queue to be printed and popped. This occurs after the next user input is taken(see main())
}

int main()
{
    vars["ShowTokens"] = "0"; //by default, the shell will not show the tokens. is ShowTokens is changed to 1 with setvar, the tokens will be printed immediately after user input.
    vars["PATH"] = "/bin:/usr/bin";//default path, can be changed when the user changes PATH with setvar
    int returnStatus = 0; // returnStatus that main should return. Its passed to done() since the funciton takes it in as a parameter.
    int runShell = 0; // integer to determine whether the shell exits out of its while loop and finishes. If it is anything other than 0, exit
    string shellPrompt = "msh > "; // declares the shell prompt the is printed every iteration unless changed by setprompt()
    const char SPACE = ' ';

    // master while loop that runs the shell
        while (runShell < 1)
        {
            cout << shellPrompt;
            string myText = " "; //string myText("some text to tokenize");
            getline(cin, myText); //takes in user input from standard input and stores it in myText
            signal(SIGCHLD, handler);
            int inputSize = 0; // will be used later to store the size of the users input

            istringstream iss(myText); // creates a stringstream named iss that now holds myText.
            string token; // creates a string to hold each individual token
            vector<string> userInput; // a string vector that will contain each newly assigned value of token

            // while getline is taking from iss, and storing everything before the delimiter ' ' space returns true , execute code
            while (getline(iss, token, SPACE))
            {
                if (token == "#"){ // if the current token is a comment indicator ignore everything after it
                    break; // breaks out of loop if comment indicator is found

                }
                if(token[0] == '^')//if the token starts out with a ^, it must be a variable
                {
                    token = token.substr(1,token.length());//removes the first char(^) from the token
                    //cout<<token<<endl;
                    if(vars.find(token) == vars.end())//if the map doesn't conatin the token
                    {
                      cout<<"No such shell variable: "<<token<< endl;//output an error
                      token = "\0"; //set the token as the empty string
                    }
                    else
                      token = vars[token]; //takes the name of the variable and replaces it with it's value in the map(vars)
                }
                if (token[0] != '\0'){
                    //cout<<"Pushing "<<token<<endl;
                    userInput.push_back(token); // fills up the userInput vector with each token, making each element of userInput a unique token of input
                }
                else {
                  return -1;
                }
            }
            inputSize = userInput.size();

            while(!completes.empty())//prints out all of the completed background processes since th last command
            {
              cout<<completes.front();
              completes.pop();
            }

            if (inputSize != 0)
            {

                if( vars["ShowTokens"] == "1") //if the variable "ShowTokens" has been set to 1 (happens through setvar), the tokens will be displayed after each shell command
                  for(int i = 0; i<inputSize; i++)
                    cout << "Token = " << userInput[i] << endl;

                // nasty if statement that executes specfic command funcitons based off the first token of the usersInput.
                if(userInput[0] == "setvar")
                  setvar(userInput, inputSize);
                else if (userInput[0] == "setprompt")
                  setprompt(shellPrompt,userInput,inputSize);
                else if(userInput[0] == "setdir")
                  setdir(userInput, inputSize);
                else if(userInput[0] == "showprocs")
                  showprocs(inputSize);
                else if(userInput[0] == "done")
                  done(returnStatus,runShell,userInput,inputSize);
                else if(userInput[0] == "run")
                  run(userInput,inputSize);
                else if(userInput[0] == "fly")
                  fly(userInput, inputSize);
                else if(userInput[0] == "tovar")
                  tovar(userInput, inputSize);
                else
                  cout<<"Invalid Command" << endl;
            }
          }
    return returnStatus;
}
