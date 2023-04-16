#define SYSCALL_SET_ORIENTATION 294

#include <signal.h>
#include <sys/syscall.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>



int main(int argc, char* argv[]){
    int degree;
    degree = atoi(argv[1]);
    syscall(SYSCALL_SET_ORIENTATION, degree);
    return 0;
}