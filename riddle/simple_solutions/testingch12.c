#include <stdio.h>
#include <unistd.h>

int main(){
    char a;
    a = 'b';
    printf("The pointer's value is %p", (void *)&a);
    fflush(stdout);
    for(int i = 0; i < 5; i++){
        sleep(2);
        if(a != 'b'){
            printf("Changed to %c",a);
            return 0;
        }
    }
    printf("never changed");
    return 1;
}