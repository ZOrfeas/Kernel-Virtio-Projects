#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <fcntl.h>
#include <sys/stat.h>


int main(){
    int file = open(".hello_there", O_RDWR);
    ftruncate(file,32768);
    return 0;
}