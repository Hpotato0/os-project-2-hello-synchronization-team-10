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
    long id = atoi(argv[1]);
    long ret = syscall(SYSCALL_ROTATION_UNLOCK, id);
    
    if(ret == 0)
        printf("[UNLOCK] id: %ld\n", id);
    else
        printf("[UNLOCK] error: %s\n", strerror(errno));
    
    return 0;
}