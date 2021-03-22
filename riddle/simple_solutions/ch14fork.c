#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(){
    for(;;){
        int pid;
        pid = fork();
        if( pid == 0 ){
            /* child process */
            int my_pid = getpid();
            if(my_pid == 32767 || my_pid == 32767){
               char* argv[] = {NULL,NULL,NULL};
               char* envp[] = {NULL,NULL,NULL};
               execve ("riddle",argv,envp);
            } else{
                exit(0);
            }
        }
        if (pid == 33000){
            return 1;
        }
    }
}