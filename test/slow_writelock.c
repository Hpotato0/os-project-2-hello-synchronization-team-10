#define SYSCALL_SET_ORIENTATION 294
#define SYSCALL_ROTATION_LOCK 295
#define SYSCALL_ROTATION_UNLOCK 296

#define ROT_READ 0
#define ROT_WRITE 1

#include <signal.h>
#include <sys/syscall.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include <errno.h>

int main(int argc, char* argv[]){
    int low, high;
    low = atoi(argv[1]);
    high = atoi(argv[2]);
    long id = syscall(SYSCALL_ROTATION_LOCK, low, high, ROT_WRITE);
    printf("[WRITE_LOCK] id: %ld degree: %d~%d\n", id,low,high);
    if(id < 0)
        printf("error during system call: %s\n", strerror(errno));
    return 0;
}