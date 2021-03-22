#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>

int main(){
	int file_desc = open("ch5.txt", O_CREAT | O_RDWR);
	dup2(file_desc, 99);
	if(fork() == 0){
		char* argv[] = {NULL,NULL,NULL};
		char* envp[] = {NULL,NULL,NULL};
		if(execve("./riddle",argv,envp) == -1){
			perror("Could not execve");
		}
	}        
	return 0;

}	
