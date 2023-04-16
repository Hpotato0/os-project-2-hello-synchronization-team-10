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

int main(int argc, char* argv[]){
    int low, high;
    long id = atoi(argv[1]);
    syscall(SYSCALL_ROTATION_UNLOCK, id);
    printf("[UNLOCK] id: %ld\n", id);
    return 0;
}