//Collaborator David Daniels
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <signal.h>

#define MAX_COMMAND_LINE_LEN 1024
#define MAX_COMMAND_LINE_ARGS 128

pid_t p;
char prompt[] = "> ";
char delimiters[] = " \t\r\n";
extern char **environ;
void sigManager(int signum){
    kill(p,SIGKILL);
}



int main() {
    // Stores the string typed into the command line.
    char command_line[MAX_COMMAND_LINE_LEN];
    char cmd_bak[MAX_COMMAND_LINE_LEN];
  
    // Stores the tokenized command line input.
    char *arguments[MAX_COMMAND_LINE_ARGS];
    	
    while (true) {
      
        do{ 
            // Print the shell prompt.
            printf("%s", prompt);
            fflush(stdout);

            // Read input from stdin and store it in command_line. If there's an
            // error, exit immediately. (If you want to learn more about this line,
            // you can Google "man fgets")
        
            if ((fgets(command_line, MAX_COMMAND_LINE_LEN, stdin) == NULL) && ferror(stdin)) {
                fprintf(stderr, "fgets error");
                exit(0);
            }
 
        }while(command_line[0] == 0x0A);  // while just ENTER pressed

      
        // If the user input was EOF (ctrl+d), exit the shell.
        if (feof(stdin)) {
            printf("\n");
            fflush(stdout);
            fflush(stderr);
            return 0;
        }

        // TODO:
        // 
        
			  // 0. Modify the prompt to print the current working directory
			      char directory[4096];
            char* gtCwd = getcwd(directory, sizeof(directory));
            printf("%s", gtCwd);
            printf("%s", directory);
            fflush(stdout);
			
        // 1. Tokenize the command line input (split it on whitespace)
        //Using command line variable from above
        char* cmdLine = command_line;

        cmdLine = strsep(&cmdLine,"\n");
        //Using veriable cmd line length above
        int t = 0;

        for(t = 0; t<MAX_COMMAND_LINE_LEN; t++){
            char* y = strsep(&cmdLine, " ");
            if (y == NULL) {
              break;
            }
            if (strchr(y, '$')!=NULL){
                memmove(y, y+1, strlen(y));
                y = getenv(y);    
                };
                arguments[t] = y;
        }
 
        char* command = arguments[0];
        int y = 1;
        // 2. Implement Built-In Commands
        if (strcmp(command, "echo") == 0){

            for(y = 1; y<MAX_COMMAND_LINE_ARGS; y++){
                if (arguments[t] == NULL){break;}
                printf("%s ", arguments[t]);
            }
        }
        else if (strcmp(command, "cd") == 0){

            if (arguments[1] == NULL){
                printf("Missing argument");
            }
            else{
                chdir(arguments[1]);
            }
            
        }else if (strcmp(command, "pwd") == 0){
       
            char path[4096];
           
           char* directory = getcwd(path, sizeof(path));
            printf("%s ", directory);
        
         }else if (strcmp(command, "exit") == 0){
            
            exit(0);
       
         }else if (strcmp(command, "env") == 0){
           
            for (y = 0; environ[t]!=NULL; y++){
                printf("%s \n", environ[t]);
            }
        
         }else if (strcmp(command, "setenv") == 0){
          
            char* variable_value[2];
            char* rest = arguments[1];
           
           for (y = 0; y<2; y++){
                char *chr = strsep(&rest, "=");
                variable_value[t] = chr;
            }
            setenv(variable_value[0], variable_value[1], 1);
       
         }else{
            
            p = fork();
        
            int isBackgroundProcess = 0;
    
            int i = 0;
          
           for (i = 0; y<MAX_COMMAND_LINE_ARGS; y++){
            
             if (arguments[t] == NULL){break;}
          }
           
           
          y--;
          
            if (arguments[y]){
                if (strcmp(arguments[y], "&") == 0){
                    isBackgroundProcess = 1;
                    arguments[y] = NULL;
              
                }else{
                    isBackgroundProcess = 0;
                }
            }
//             
            int status;
           
           
           if (p <0){
              
             printf("ERROR: Fork");
                exit(1);
            
           }else if (p == 0){
               
             if (execvp(arguments[0], arguments) == -1){
                    printf("execvp ERROR: No such file or directory");
                }
      
    
       signal(SIGINT, sigManager);
       exit(1);
    }
    
    }
    
        // 3. Create a child process which will execute the command line input
        pid_t pid = fork();
  if (pid < 0) {
    perror("fork failed");
    return 1;
  } else if (pid == 0) {
    //printf("i am a child process, with pid: %d\n", (int)getpid());
    execvp(arguments[1], arguments + 1);
    // This can only be reached if execvp returns (i.e. fails to exec)
    perror("execvp failed");
    exit(123);
  }

  // 4. The parent process should wait for the child to complete unless its a background process

  //printf("i am parent of %d\n", pid);
  int status; 
 
  pid_t child_pid = wait(&status);
  if(child_pid < 0) {
    perror("wait4 failed");
  } else {
    //printf("child exited with %d\n", status);
  }
 
  
        // 4. The parent process should wait for the child to complete unless its a background process
      
      
        // Hints (put these into Google):
        // man fork
        // man execvp
        // man wait
        // man strtok
        // man environ
        // man signals
        
        // Extra Credit
        // man dup2
        // man open
        // man pipes
    }
    // This should never be reached.
    return -1;
}