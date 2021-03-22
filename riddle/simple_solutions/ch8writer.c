#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <fcntl.h>
#include <string.h>
int main(int argc, char *argv[]){
	if (argc != 2){
		printf("Exactly one file should be given");
		return 1;
	}
	int bf_file = open(argv[1], O_WRONLY);
	lseek(bf_file,1073741820,SEEK_SET);
	write(bf_file,"111111111111111111111111",strlen("111111111111111111111111"));
	close(bf_file);
	return 0;
}
