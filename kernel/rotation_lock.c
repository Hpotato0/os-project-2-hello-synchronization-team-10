#include <linux/syscalls.h>
#include <linux/spinlock.h>
#include <linux/list.h>
#include <linux/semaphore.h>
#define ROT_READ 100
#define ROT_WRITE 101

// global variable
int orientation;
DEFINE_RWLOCK(orientation_lock);
DEFINE_RWLOCK(list_lock);
DEFINE_RWLOCK(state_lock);

int access_state[360];

struct thread_node {
    struct list_head list;
    int type;
    int low;
    int high;
    struct semaphore start;
    long id;
};
LIST_HEAD(thread_list);

int is_degree_in_range(int degree, int low, int high)
{
    return ((low <= high && low <= degree && degree <= high) || (low >= high && (high <= degree || degree <= low)));
}
long set_orientation (int degree){
    if(degree < 0 || degree >= 360)
        return -EINVAL;

    write_lock(&orientation_lock);
    orientation = degree;
    write_unlock(&orientation_lock);

    read_lock(&orientation_lock);
    write_lock(&list_lock);
    write_lock(&state_lock);

    //1st traversal
    struct thread_node* pos;
    list_for_each_entry(pos, &thread_list, list){
        int low = pos->low;
        int high = pos->high;
        if(pos->type == ROT_WRITE && is_degree_in_range(degree, low, high)){
            int start = 1;
            if(low <= high){
                for(int idx = low; idx <= high; idx++){
                    if(access_state[idx] != 0){
                        start = 0;
                        break;
                    }
                }
            }
            else{
                for(int idx = high; idx < 360; idx++){
                    if(access_state[idx] != 0){
                        start = 0;
                        break;
                    }
                }
                if(start){
                for(int idx = 0; idx <= low; idx++){
                    if(access_state[idx] != 0){
                        start = 0;
                        break;
                    }
                }
                }
            }

            if(start)
                up(&pos->start)
            if(low <= high){
                for(int idx = low; idx <= high; idx++)
                    access_state[idx]--;
            }
            else{
                for(int idx = high; idx < 360; idx++)
                    access_state[idx]--;
                for(int idx = 0; idx <= low; idx++)
                    access_state[idx]--;
            }
        }
    }

    list_for_each_entry(pos, &thread_list, list){
        int low = pos->low;
        int high = pos->high;
        if(pos->type == ROT_READ && is_degree_in_range(degree, low, high)){
            int start = 1;
            if(low <= high){
                for(int idx = low; idx <= high; idx++){
                    if(access_state[idx] < 0){
                        start = 0;
                        break;
                    }
                }
            }
            else{
                for(int idx = high; idx < 360; idx++){
                    if(access_state[idx] < 0){
                        start = 0;
                        break;
                    }
                }
                if(start){
                for(int idx = 0; idx <= low; idx++){
                    if(access_state[idx] < 0){
                        start = 0;
                        break;
                    }
                }
                }
            }

            if(start)
                up(&pos->start)
            if(low <= high){
                for(int idx = low; idx <= high; idx++)
                    access_state[idx]++;
            }
            else{
                for(int idx = high; idx < 360; idx++)
                    access_state[idx]++;
                for(int idx = 0; idx <= low; idx++)
                    access_state[idx]++;
            }
        }
    }

    write_unlock(&state_lock);
    write_unlock(&list_lock);
    read_unlock(&orientation_lock);
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