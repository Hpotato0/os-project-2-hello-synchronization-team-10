#define _GND_SOURCE
#include <stdlib.h>
#include <stdio.h>
#include <sys/syscall.h>
#include <unistd.h>
#include <ctype.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <signal.h>
#include <math.h>

#define SYS_ROT_LOCK 295
#define SYS_ROT_UNLOCK 296
#define READ 0
#define WRITE 1

long lock_id;
int fd;
int temp;


void primefactorial(int num, int low, int high){
    int start = 1;

    printf("student-%d-%d: %d = ",low, high, num);
    if(num < 2) {
        printf("%d\n",num);
        return;
    }
    while(num % 2 == 0) {
        if(!start) {
            printf(" * ");
        }
        else start = 0;
        printf("2");
        num = num / 2;
    }

    for (int i = 3; i <= sqrt(num); i = i + 2) {
        while (num % i == 0) {
            if (!start) {
                printf(" * ");
            }
            else start = 0;
            printf("%d", i);
            num = num / i;
        }
    }

    if (num > 2) {
        if(start) printf("%d\n", num);
        else printf(" * %d\n", num);
    }
    else printf("\n");
}

void INThandler(int sig){
    close(fd);
    syscall(SYS_ROT_UNLOCK, lock_id); 
    printf("############## Student stops to slove quizs ##############\n");
    exit(0);
}

int main(int argc, char* argv[]){
    int low, high;
    char buf[1024];
    int num;
    signal(SIGINT, INThandler);
    printf("############## Studnet starts to slove quizs! ##############\n");

    if(argc == 3){
        low = atoi(argv[1]);
        high = atoi(argv[2]);
    }
    else{
        printf("invalid number of arguments\n");
        return -1;
    }

    while(1) {
        lock_id = syscall(SYS_ROT_LOCK, low, high, READ);
        fd = open("quiz", O_RDONLY);
        read(fd, buf, 1024);
        num = atoi(buf);
        primefactorial(num, low, high);
        close(fd);       
        syscall(SYS_ROT_UNLOCK, lock_id);
        sleep(1);
    }

    return 0;
}
