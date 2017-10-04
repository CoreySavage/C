//
//  main.c
//  Program3
//  smallsh
//
//  Created by Corey Savage on 11/10/16.
//
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <fcntl.h>
#include <stdbool.h>

// Global Variables
char line[2500];                        // inputted by user
int statusNum;                          // tracks last exited background process
int i;
int backgroundProcesses[100];          // background PID
int backgroundProcessesNum = 0;
bool redirection = false;

// Got idea from Discussion boards
// stores all relevant user inputted information
typedef struct userInput {
    char* command;
    char* arguments[550];
    int argumentNum;
    char* inputFile;
    char* outputFile;
    bool background;
} UserInput;

// Function prototypes
void userInputParse(UserInput *);

void getLine();

bool isBuiltInCmd(UserInput *);

void checkProcesses();

void handleRedirection(UserInput *);


int main(int argc, const char * argv[]) {
    
    bool continueShell = true;
    UserInput input;
    bool newInput = false;                  // when true, get new input from user
    
    // Loop through untill continueShell is false
    do {
        
        // Reset all data in UserInput struct, for new line
        input.command = NULL;
        input.argumentNum = 0;
        for (i=0; i<550; i++) {
            input.arguments[i] = NULL;
        }
        input.background = false;
        input.inputFile = NULL;
        input.outputFile = NULL;
        
        // Checks for any background Processes that have been terminated
        checkProcesses();

        // Get user input
        getLine();
        
        // Error check - max command line length
        if (strlen(line) > 2049) {
            printf("Too long of a command! %lu (max 2048)\n", strlen(line));
            newInput = true;
        }
        
        // Checks and displays comments (input that starts with #)
        if (line[0] == '#') {
            printf("%s\n", line);
            newInput = true;
        }
        
        // Parses user input into Struct UserInput
        userInputParse(&input);
        
        // Error checking - max arguments
        if (input.argumentNum > 512) {
            printf("Too many arguments! %d (max 512)\n", input.argumentNum);
            newInput = true;
            
        }
        
        // Handle built in commands (exit, cd, status)
        if (isBuiltInCmd(&input)) {
            
            // exit
            if (strcmp(input.command, "exit") == 0) {
                continueShell = false;
                break;
                
            }
            
            // cd - change directory
            if (strcmp(input.command, "cd") == 0) {
                // No path given - change directory to HOME
                if (input.arguments[1] == NULL) {
                    chdir(getenv("HOME"));
                }
                // Change directory according to path inputted by user
                else {
                    char workingDirectory[2048];
                    getcwd(workingDirectory, sizeof(workingDirectory));
                    strcat(workingDirectory, "/");                              // Add to end of current directory
                    strcat(workingDirectory, input.arguments[1]);               // arguments[0] holds the command for execpv()
                    chdir(workingDirectory);
                }
            }
            
            // status
            if (strcmp(input.command, "status") == 0) {
                printf("exit value %d\n", statusNum);
            }
        }
        
        // Non built in command, comment, or blank line - with no size errors
        else if (newInput == false && input.command != NULL) {
            
            pid_t pid;
            
            // Background processes
            if (input.background == true) {
                if ((pid = fork()) < 0) {
                    perror("Error while forking");
                    exit(1);
                }
                // Child process
                if (pid == 0) {
                    
                    // If input calls for redirection, determine output or input and redirect command
                    handleRedirection(&input);
                        
                    // No input or output redirection
                    if (redirection == 0) {
                        // Run command
                        execvp(input.command, input.arguments);
                    }
                    // Error checking - cannot find file or directory
                    printf("%s no such file or directory\n", input.command);
                    exit(1);
                }
                // Parent process
                else {
                    int status;
                    int process;
                    printf("background pid is %d\n", pid);
                    // Store background process id
                    backgroundProcesses[backgroundProcessesNum] = pid;
                    backgroundProcessesNum++;
                    // Wait for background process
                    process = waitpid(pid, &status, WNOHANG);
                    continue;
                }
            }
            // Foreground processes
            else {
                if ((pid = fork()) < 0) {
                    perror("Error while forking");
                    exit(1);
                }
                // Child process
                if (pid == 0) {

                    // If input calls for redirection, determine output or input and redirect command
                    handleRedirection(&input);
                    
                    // No input or output redirection
                    if (redirection == 0) {
                        // Run command
                        execvp(input.command, input.arguments);
                    }
                    // Error checking - cannot find file or directory
                    printf("%s no such file or directory\n", input.command);
                    exit(0);
                }
                // Parent process
                else {
                    int status;
                    // Wait for background process
                    waitpid(pid, &status, 0);
                    // Store status number when background process is terminated
                    if (WIFEXITED(status)) {
                        statusNum = WEXITSTATUS(status);
                    }
                }
            }
        }
        signal(SIGINT, SIG_IGN);
        
    } while (continueShell);
    
    return 0;
}


// Function flushes stdout and stdin, prompts user for input, and gets input.
// Also, changes last char in input from \n (newline) to \0 (null)
void getLine() {
    
    fflush(stdout);
    fflush(stdin);
    printf(": ");
    fflush(stdin);
    if (fgets(line, sizeof(line), stdin) != NULL) {
        char *pos;
        // Search for newline char
        pos=strchr(line, '\n');
        // Change newline to null
        *pos = '\0';
    }

    return;
}

// Parses out inputted line to UserInput Struct
// Stores command, arguments in an array (arguments[0] is the command), output and input files, and background processes (ends with &)
void userInputParse(UserInput* inputIn) {
    
    char* parsedLine;
    i = 1;                                              // arguments begin at arguments[1]
    
    parsedLine = strtok(line, " ");                     // Creates tokens
    inputIn->command = parsedLine;
    inputIn->arguments[0] = parsedLine;
    parsedLine = strtok(NULL, " ");                     // Next token
    
    // Loops until end of user input
    while (parsedLine != NULL) {
        // Input file indication
        if (strcmp(parsedLine, "<") == 0) {
            // Get input file
            parsedLine = strtok(NULL, " ");
            inputIn->inputFile = parsedLine;
        }
        // Output file indication
        else if (strcmp(parsedLine, ">") == 0) {
            // Get output file
            parsedLine = strtok(NULL, " ");
            inputIn->outputFile = parsedLine;
        }
        // Background process indication
        else if (strcmp(parsedLine, "&") == 0) {
            
            inputIn->background = true;
        }
        // Arguments
        else {
            inputIn->argumentNum += 1;
            inputIn->arguments[i] = parsedLine;
            i++;
        }
        // Next token
        parsedLine = strtok(NULL, " ");
    }
    
    return;
}

// Checks UserInput input for built in commands. Returns true if there are
bool isBuiltInCmd(UserInput* inputIn) {
    
    if (inputIn->command != NULL) {
        if (strcmp(inputIn->command, "exit") == 0 || strcmp(inputIn->command, "cd") == 0 || strcmp(inputIn->command, "status") == 0) {
       
            return true;
        }
    }
    
    return false;
    
}

// Run before new input to check for recently terminated background processes
void checkProcesses() {
    
    int status;
    
    for (i=0; i<backgroundProcessesNum; i++) { //Run through the array of process ids
        if (waitpid(backgroundProcesses[i], &status, WNOHANG) > 0) {
            // Exited
            if (WIFEXITED(status)) {
                printf("Background pid %d is done: exit value %d\n", backgroundProcesses[i], WEXITSTATUS(status));
            }
            // Terminated by signal
            if(WIFSIGNALED(status)) {
                printf("Background pid %d is done: terminated by signal %d\n", backgroundProcesses[i], WTERMSIG(status));
            }
        }
    }
}

// If UserInput has an input or outfile to redirect to, redirect to file and run command
void handleRedirection(UserInput* inputIn) {
    
    // Input redirection
    if (inputIn->inputFile != NULL) {
        // Error checking - file cannot be opened
        if (access(inputIn->inputFile, R_OK) == -1) {
            printf("cannot open %s for input\n", inputIn->inputFile);
            redirection = true;
        }
        // File can be opened successfully
        else {
            int fd = open(inputIn->inputFile, O_RDONLY, 0);
            dup2(fd, STDIN_FILENO);
            close(fd);
            redirection = true;
            // Run command
            execvp(inputIn->command, inputIn->arguments);
        }
    }
    // Output redirection
    if (inputIn->outputFile != NULL) {
        // Create file
        int fd = creat(inputIn->outputFile, 0644);
        dup2(fd, STDOUT_FILENO);
        close(fd);
        redirection = true;
        // Run command
        execvp(inputIn->command, inputIn->arguments);
    }
    
    return;
}






