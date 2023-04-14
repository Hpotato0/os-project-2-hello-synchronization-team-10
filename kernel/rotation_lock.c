#include <linux/syscalls.h>
#include <linux/spinlock.h>
#include <linux/list.h>
#include <linux/semaphore.h>
#define ROTAION_LOCK_READ 100
#define ROTATION_LOCK_WRITE 101
// global variable

int orientation;
rwlock_t orientation_lock;
rwlock_t list_lock;
rwlock_t state_lock;

int access_state[360];

struct thread_list {
    int type;
    int low;
    int high;
    sem_t start;
    long id;
    struct list_head *next;
    struct list_head *prev;
}
struct thread_list head;

int is_degree_in_range(int degree, int low, int high)
{
    return ((low <= high && low <= degree && degree <= high) || (low >= high && high <= degree && degree <= (low + 360)));
}
long set_orientation (int degree){
    static int isinit = 0;

}

long rotation_lock(int low, int high, int type){
    int local_orienation;
    static long id = 0;
    int start = 1;

    //(1) Struct dynamic alloc and init
    struct thread_list *new_thread = (thread_list*)malloc(sizeof(struct thread_list));
    new_thread -> type = type;
    new_thread -> low = low;
    new_thread -> high = high;

    //(2) Check R/W locks & orientation, then set start
    read_lock(&orientation_lock);
    local_orientation = orientation;
    read_unlock(&orientation_lock);

    read_lock(&state_lock);
    if(local_orienation <= )
    {
        for()
    }
}

long rotation_unlock(long id){

}