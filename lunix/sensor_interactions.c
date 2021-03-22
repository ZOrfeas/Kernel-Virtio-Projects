#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <inttypes.h>

#define ALL_SENSORS -10
#define ALL_MEASUREMENTS -20
#define ALL_BYTES -30
//////////////////////////
char *_h = "-h";
char *__help = "--help";
char *_o = "-o";
char *__overview = "--overview";
char *_s = "-s";
char *__sensor = "--sensor";
char *_m = "-m";
char *__measurement = "--measurement";
char *_b = "-b";
char *__byte = "--byte";
char *_r = "-r";
char *_raw = "--raw";
char *_mm = "-m";
char *__mmap = "--map";
//////////////////////////
char cooked = 0x01;
char mmaped = 0x00;

int sensor = ALL_SENSORS;
int measurement = ALL_MEASUREMENTS;
int byte = ALL_BYTES;
enum types{batt = 0, temp, light, types_end};
static const char *typesstr[types_end] = {"batt", "temp", "light"};

char *data_types[3] = 
{
    "Battery",
    "Temperature",
    "Light"
};

char *files[6] =
{
    "/dev/lunix0-batt",
    "/dev/lunix0-temp",
    "/dev/lunix0-light",
    "/dev/lunix1-batt",
    "/dev/lunix1-temp",
    "/dev/lunix1-light"
};

int check_types(char *poss_type) {
    int ret;
    enum types i;
    for (i = 0; i < types_end; ++i) {
        if(strcmp(poss_type, typesstr[i]) == 0) break;
    }
    if (i == types_end) {
        return -1;
    } else {
        return (int) i;
    }
}

void help_message() {
    printf(
"USAGE: ./sensor_interactions OPTION [OPTIONS]"
"User space command line utility providing a set of interactions with the Lunix device.\n\t"
"-h, --help                  prints this help message.\n\t"
"-o, --overview              gives all current measurements of the (two) available sensors.\n\t"
"-s, --sensor                takes a sensor number (0,1 only at the moment available) and\n\t" 
"                            prints that sensor's measurements (e.g: -s 1).\n\t"
"-m, --measurement           takes a measurement type and prints that measurement, if used\n\t"
"                            with the -s option will only give one sensor's measurement.\n\t"
"-b, --byte                  specify which character to print (will only read up to that from driver)\n\t"
"                            1-5 for safety reasons :) \n\t"
"-r, --raw                   (not implemented) specifies that the data be printed in raw form.\n\t"
"-mm, --mmap                 (not implemented) will use mmap() to accomplish the tasks.\n");
}

char read_one_byte(char *filename, int byte_pos) {
    char retval;
    int fd, count;
    char local_buff[20];
    fd = open(filename, O_RDONLY);
    // printf("filename is %s\n", filename);
    // printf("byte_pos + 1 =  %d\n", byte_pos+1);
    count = read(fd,local_buff,byte_pos+1);
    // printf("count = %d \n",count);
    // printf("local_buff[byte_pos] here is %c\n", local_buff[byte_pos]);
    retval = local_buff[byte_pos];
    close(fd);
    return retval;
}

float read_one_full(char *filename) {
    float retval;
    int fd, count;
    char local_buff[20];
    fd = open(filename, O_RDONLY);
    count = read(fd,local_buff,20);
    retval = strtod(local_buff,NULL);
    close(fd);
    return retval;
}

void one_of_each() {
    int i;
    float measurement;
    printf("\e[4m\e[1mLunix sensor 0 gave values:\e[0m\n");
    for(i = 0; i < 3; i++) {
        measurement = read_one_full(files[i]);
        printf("\e[4m%s:\e[0m %.6f\n", data_types[i % 3], measurement);
    }
    printf("\e[4m\e[1mLunix sensor 1 gave values:\e[0m\n");
    for(i = 3; i < 6; i++) {
        measurement = read_one_full(files[i]);
        printf("\e[4m%s:\e[0m %.6f\n", data_types[i % 3], measurement);
    }
}



void do_stuff() {
    int limit;
    int chosen[6];
    if(sensor == ALL_SENSORS && measurement == ALL_MEASUREMENTS && byte == ALL_BYTES) {
        one_of_each();
        return;
    } else if (sensor == ALL_SENSORS && measurement != ALL_MEASUREMENTS) {
        chosen[0] = 0 + measurement;
        chosen[1] = 3 + measurement;
        limit = 2;
    } else if (sensor != ALL_SENSORS && measurement == ALL_MEASUREMENTS) {
        chosen[0] = (sensor*3) + 0;
        chosen[1] = (sensor*3) + 1;
        chosen[2] = (sensor*3) + 2;
        limit = 3;
    } else if (sensor != ALL_SENSORS && measurement != ALL_MEASUREMENTS) {
    	// printf("Mphka \n");
        chosen[0] = (sensor*3) + measurement;
        limit = 1;
    } else {
        chosen[0] = 0; chosen[1] = 1; chosen[2] = 2;
        chosen[3] = 3; chosen[4] = 4; chosen[5] = 5;
        limit = 6;
    }
    if(byte == ALL_BYTES){
        for(int i = 0; i < limit; i++) {
            printf("%d) %.6f\n", i, read_one_full(files[chosen[i]]));
        }
    } else {
        for(int i = 0; i < limit; i++) {
            printf("%d) %c\n", i, read_one_byte(files[chosen[i]], byte));
        }
    }
    return;
}

int main(int argc, char *argv[]) {

    int snsr_nr, msr, byte_pos;
    if(argc == 1) {
        help_message();
        return 0;
    }
    if(!strcmp(argv[1],"demo")){
        int fd,count;
        fd = open("/dev/lunix0-temp", O_RDONLY);
        
        if(!fork()) {
            char buffer[20];
            read(fd,buffer,2);
            printf("child: %c %c \n", buffer[0], buffer[1]);
            _exit(0);
        } else {
            char buffer[20];
            int status;

            // wait(&status);
            read(fd,buffer,4);
            printf("parent: %c %c %c %c \n", buffer[0], buffer[1], buffer[2], buffer[3]);
            close(fd);
            return 0;
        }
    } else if (!strcmp(argv[1],"mmap")) {
    	int fd;
        fd = open("/dev/lunix0-temp", O_RDONLY);
        size_t pagesize =(unsigned long)getpagesize();
        char *mapped_space;
        // uint32_t *mapped_space;
        if(!(mapped_space =(char *) mmap(NULL, pagesize, PROT_READ, MAP_SHARED, fd,0)))
            printf("mmap failed\n");
        // uint32_t *values = (uint32_t *) mapped_space;
        
        // printf("%" PRIu32 "\n",mapped_space[0]);
        // printf("%" PRIu32 "\n",mapped_space[1]);
        // printf("%" PRIu32 "\n",mapped_space[2]);
        printf("%hhx\n",mapped_space[0]);
        printf("testtesttest\n");
        printf("%hhx\n",mapped_space[1]);
        printf("%hhx\n",mapped_space[2]);
        printf("%hhx\n",mapped_space[3]);
        munmap(mapped_space,pagesize);
        close(fd);
        return 0;

    } else {
        for(int i = 1; i < argc; i++) {
            // printf("%d\n",i);
            if(!strcmp(argv[i], _h) || !strcmp(argv[i], __help)) {
                help_message();
                return 0;
            } else if (!strcmp(argv[i], _o) || !strcmp(argv[i], __overview)) {
                sensor = ALL_SENSORS;
                measurement = ALL_MEASUREMENTS;
                byte = ALL_BYTES;
                break;
            } else if (!strcmp(argv[i], _s) || !strcmp(argv[i], __sensor)) {
                // printf("Test Test Test %s, %d\n",argv[i],isdigit(argv[i]));
                if (i+1 < argc && isdigit(*argv[++i]) && ((snsr_nr = atoi(argv[i])) < 2)) {
                    // printf("TEsTinG\n");
                    sensor = snsr_nr;
                    continue;
                } else {
                    help_message();
                    return 1;
                }
            } else if (!strcmp(argv[i], _m) || !strcmp(argv[i], __measurement)) {
                if (i+1 < argc && ((msr = check_types(argv[++i])) != -1)) {
                    measurement = msr;
                } else {
                    help_message();
                    return 1;
                }
            } else if (!strcmp(argv[i], _b) || !strcmp(argv[i], __byte)) {
                if (i+1 < argc && isdigit(*argv[++i]) && ((byte_pos = atoi(argv[i])) < 5) && byte_pos >= 1) {
                    byte = byte_pos - 1;
                    continue;
                } else {
                    help_message();
                    return 1;
                }
            }
        }
        // printf("sensor = %d || measurement = %d || byte = %d\n", sensor, measurement, byte);
        do_stuff();
        return 0;
    }
    /////////////////////////////////////////////////////////////////////////////////////////

    /////////////////////////////////////////////////////////////////////////////////////////
}
