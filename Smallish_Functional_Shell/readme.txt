Read Me - Smallsh
COREY SAVAGE
________________


This program was tested on OSU’s server with a Mac and a PC




OVERVIEW -
________________
This program is a small and functional shell. The shell allows for the redirection of standard input and standard output and supports both foreground and background processes.


The shell supports three built in commands: exit, cd, and status. It also supports comments, which are lines beginning with # (hashtag).


The general syntax of a command line is:

        command [arg1 arg2 ...] [< input_file] [> output_file] [&]




COMPILE - 
________________
With the included makefile, compiling the c file can be done in a couple simple steps
        
1. Download the file to you local computer or desired server.
2. Open up your favorite command-line tool.
3. Go to the directory that the files are currently located.
4. Type make main.c 


You c file will be compiled from the makefile.




EXECUTE -
________________
To execute the program, enter: Smallsh






CONTROLS - 
________________
        
        exit                -                Exits program
cd                -                Changes directories
status                -                Exit status of last foreground process






RESTRICTIONS -
________________


        2048 Character                -                Max command line length
        512 Arguments                -                Max arguments