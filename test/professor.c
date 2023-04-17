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

#define SYS_ROT_LOCK 295
#define SYS_ROT_UNLOCK 296
#define READ 0
#define WRITE 1

long lock_id;
int fd;
int temp;

void INThandler(int sig){

    close(fd);
    syscall(SYS_ROT_UNLOCK, lock_id); 
    printf("############## Finally Professor stops to make quizs ##############\n");
    exit(0);
}

int main(int argc, char* argv[]){
    int counter;
    
    char buf[1024];
    printf("############## Professor starts to make quizs! ##############\n");
    if(!isdigit((*argv[1])))
    {
        printf("Argument should be a integer");
        return 0;
    }

    if(argc == 2){
        counter = atoi(argv[1]);
    }
    else{
        printf("invalid number of arguments\n");
        return -1;
    }

    signal(SIGINT, INThandler);

    while(1) {
        lock_id = syscall(SYS_ROT_LOCK, 0, 180, WRITE);
        fd = open("quiz", O_RDWR | O_TRUNC);
        sprintf(buf, "%d", counter);
        write(fd, buf, strlen(buf));
        close(fd);       
        printf("professor: %d\n", counter);
        counter++;
        syscall(SYS_ROT_UNLOCK, lock_id);
    }

    return 0;
}
