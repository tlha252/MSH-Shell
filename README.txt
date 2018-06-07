Files Included:
	msh.cpp
	Makefile
	README

Group Members:
	Thomas Hays
	John Connor Davis

Command to compile:
	g++ -Wall msh.cpp -o msh (compiled this way in the makefile)

Output EXE:
	msh

General Description of Project:
	The goal of this program was to write a shell in C++ that allows users to
	interactively execute Unix programs. My Shell (msh) reads in commands
	typed by the user into a vector<string> that tokenizes the input in
	order to dilleniate each element of input. Each token is passed to corresponding
	conditional statements that determines what was asked of the shell (either
	control commands or built in commands). If the user selects one of the program
	control commands, fork() and execvp() are utilized to create copies of the shell
	process and allows a child process to execute the passed program while the parent
	continues the process of the shell itself. If one of the built-in commands are selected
	msh modifies local variables within the cpp such as: the printed prompt, stored variables
	, and list of running processes to execute the specifications of said command. msh
	continues to ask for user input until done is selected and returns with the specified
	parameter.

Special Features/Limitations:
	When Tovar() takes in input from standard output to store in a specific variable,
	it stores an additonal newline at the end of the input with it. This does not
	affect the overall functionality of Tovar() but this can be displayed if the
	stored variable is printed. Example:

	INPUT: Tovar STORE ls
	OUTPUT: msh > msh.cpp msh
	INPUT: run echo ^STORE
	OUTPUT:
	msh.cpp
	msh

	END OF EXAMPLE

	Another thing to note is that this program does utilize global variables. This is
	generally a poor programming practice but is necessary when implementing a signal handler
	the way the msh does. No parameters can be passed to signal handler so in order for that
	function to edit anything it has to access them from the global declaration. This does
	not affect the overall functionality of the program

	If the user tries to reference a stored variable with ^ but the variable has not been set/does not exist
	the program, the program reads an error message and treats it as an empty character passed but after the program
	immediately exits. The reason for this is unknown but the empty character is still passed to the specified input.

This concludes the README file.
