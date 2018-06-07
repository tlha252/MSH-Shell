# MSH-Shell

Description of Project:
    The goal of this program is to write a shell in C++ that allows users to
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

Files Included:
	    msh.cpp
	    Makefile
	    README

Command to compile:
	    g++ -Wall msh.cpp -o msh (compiled this way in the makefile)

Output EXE:
	    msh

Commands you can enter:

---------------------------------------------------------------BUILT IN COMMANDS----------------------------------------------------------------- 
    setvar variable value
        variable is any reasonable variable name (starting with a letter, continuing with letters or numbers), value is a token. Variable names are case-sensitive. The effect of this command is to associate the variable name with the given value in a data structure inside msh.
    
    setprompt newPrompt
        Set the shell prompt to newPrompt, which is a token. The initial prompt in msh is msh > .

    setdir directoryName
        This command changes the current directory to directoryName. The required parameter directoryName may be either absolute (starting with /) or relative (not starting with /).

    showprocs
        msh lists all processes running in the background.

    done value
        msh exits. If the value parameter is present, it must be a non-negative integer, in which case msh uses that integer as its exit status. If the parameter is absent, msh exits with status 0. msh also accepts <control-D> (end-of-file) on the input stream and treats it like done 0.
--------------------------------------------------------------END OF BUILT IN COMMANDS-----------------------------------------------------------

------------------------------------------------------PROGRAM CONTROL COMMANDS------------------------------------------------------------------

    run cmd [param ... ]
        (The brackets indicate "optional" and the dots indicate "and more if desired".) The user submits a run command to execute a program. The keyword run must appear as the first token on the command line, followed by the command to be executed: cmd is a token that specifies the filename of the program the user wants to execute. It is followed by zero or more tokens specifying parameters. msh waits for the program to finish before it prompts for and accepts the next command.If cmd starts with a / character, it is a full path name starting at the root of the filesystem. A cmd that starts with ./ is a path name starting in the current directory. Otherwise, msh looks for cmd in a list of directories indicated by the variable PATH, whose value is a colon-separated list of directories. By default, PATH has value /bin:/usr/bin; the user can change that value by assigning a new value to the variable PATH.

    fly cmd [param ... ]
        The fly command is identical to the run command, except that the process running the command runs in the background. msh immediately prompts for and accepts the next command.

    tovar variable cmd [param ... ]
        The tovar command executes the program cmd along with its parameters, if any, just like the run command. However, msh absorbs the standard output of the program and assigns it as the value of variable specified by the second token.

----------------------------------------------------END OF PROGRAM CONTROL COMMANDS--------------------------------------------------------------



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