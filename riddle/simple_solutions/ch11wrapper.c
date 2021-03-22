#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>

int main(){
    int file_handle = open("secret_number",  O_RDWR|O_CREAT|O_TRUNC);
    if(fork() == 0){
        char* argv[] = {NULL,NULL,NULL};
        char* envp[] = {NULL,NULL,NULL};
        if (execve("./riddle",argv,envp) == -1){
            perror("Could not execve");
        }
    }
    sleep(3);
    int cheat = open("cheat", O_CREAT | O_WRONLY);
    char buff[256];
    read(file_handle,buff,255);
    write(cheat,buff,255);
    close(file_handle);
    close(cheat);
    return 0;
}