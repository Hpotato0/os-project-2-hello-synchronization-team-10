#include <linux/syscalls.h>
#include <linux/spinlock.h>
#include <linux/list.h>
// global variable

rwlock_t orientation_lock;

struct thread_list {
    int type;
    int low;
    int high;
    int start;
    long id;
    struct list_head *next;
    struct list_head *prev;
}
LIST_HEAD(thread_list);

long set_orientation (int degree){

}

long rotation_lock(int low, int high, int type){

}

long rotation_unlock(long id){

}