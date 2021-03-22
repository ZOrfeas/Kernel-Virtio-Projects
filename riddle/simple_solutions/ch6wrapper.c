#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>

int main(){
	int fds1[2];
	if(pipe(fds1) < 0){
		perror("Failed to Create Pipe");
		return 1;
	}
	dup2(fds1[0],33);
	dup2(fds1[1],1);
	dup2(fds1[1],34);
	dup2(fds1[0],53);
	dup2(fds1[1],54);
	if(fork() == 0){
		char* argv[] = {NULL,NULL,NULL};
		char* envp[] = {NULL,NULL,NULL};
		if(execve("./riddle",argv,envp) == -1){
			perror("Could not execve");
		}
	}
	return 0;
}
