#include <linux/syscalls.h>
#include <linux/spinlock.h>
#include <linux/list.h>
#include <linux/semaphore.h>
#include <linux/slab.h>
#define ROT_READ  0
#define ROT_WRITE 1

// global variable
static int orientation;
static int access_state[360];

DEFINE_RWLOCK(orientation_lock);
DEFINE_RWLOCK(state_lock);
DEFINE_RWLOCK(list_lock);

struct thread_node {
    struct list_head list;
    int type;
    int low;
    int high;
    struct semaphore start;
    long id;
};
LIST_HEAD(thread_list);


static void display_current_state(){
    int counter = 0;
    int i = 0;
    int j = 0;
    printk("[DISPLAY_CURRNET_STATE]\n");
    list_for_each_entry(pos, &thread_list, list){
        printk("THREAD [%d]- type: %d low: %d high: %d id: %ld\n",counter++, pos->type, pos->low, pos->high, pos->id);
    }
    printk("ACCESS STATE\n");
    for(i = 0; i < 36; i++){
        printk("%d ~ %d: %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d", i*10, i*10+10, 
        access_state[i], access_state[i+1], access_state[i+2], access_state[i+3], access_state[i+4], access_state[i+5], access_state[i+6], access_state[i+7], access_state[i+8], access_state[i+9]);
    }

}


static int is_degree_in_range(int degree, int low, int high) 
{
    return ((low <= high && low <= degree && degree <= high) || (low >= high && (high <= degree || degree <= low)));
}

static void traverse_twice_give_rotlock(void)
{
    struct thread_node* pos;
    int idx;
    
    // 1st traversal
    list_for_each_entry(pos, &thread_list, list){
        int low = pos->low;
        int high = pos->high;
        // check if write lock should be granted: (1)is degree in range?
        if(pos->type == ROT_WRITE && is_degree_in_range(orientation, low, high)){
            // check if write lock should be granted: (2)any overlapping reads/writes?
            int start = 1;
            if(low <= high){
                for(idx = low; idx <= high; idx++){
                    if(access_state[idx] != 0){start = 0; break;}}
            }
            else{
                for(idx = high; idx <= (low + 360); idx++){
                    if(access_state[(idx%360)] != 0){start = 0; break;}}
            }

            // change access_state if write lock is granted
            if(start){
                up(&pos->start);
                if(low <= high){
                    for(idx = low; idx <= high; idx++)
                        access_state[idx] = -1;
                }
                else{
                    for(idx = high; idx <= (low+360); idx++)
                        access_state[(idx%360)] = -1;
                }
            }
        }
    }

    // 2nd traversal
    list_for_each_entry(pos, &thread_list, list){
        int low = pos->low;
        int high = pos->high;

        // check if read lock should be granted: (1)is degree in range?
        if(pos->type == ROT_READ && is_degree_in_range(orientation, low, high)){
            // check if read lock should be granted: (2)any overlapping writes?
            int start = 1;
            if(low <= high){
                for(idx = low; idx <= high; idx++){
                    if(access_state[idx] < 0){start = 0; break;}}
            }
            else{
                for(idx = high; idx <= (low + 360); idx++){
                    if(access_state[(idx%360)] < 0){start = 0; break;}}
            }

            // change access_state if read lock is granted
            if(start){
                up(&pos->start);
                if(low <= high){
                    for(idx = low; idx <= high; idx++)
                        access_state[idx]++;
                }
                else{
                    for(idx = high; idx <= (low+360); idx++)
                        access_state[(idx%360)]++;
                }
            }
        }
    }
}

SYSCALL_DEFINE1(set_orientation, int, degree){
    if(degree < 0 || degree >= 360)
        return -EINVAL;

    write_lock(&orientation_lock);
    orientation = degree;
    write_unlock(&orientation_lock);

    read_lock(&orientation_lock);
    write_lock(&list_lock);
    write_lock(&state_lock);

    traverse_twice_give_rotlock();

    write_unlock(&state_lock);
    write_unlock(&list_lock);
    read_unlock(&orientation_lock);

    return 0;
}

SYSCALL_DEFINE3(rotation_lock, int, low, int, high, int, type){
    int local_orientation;
    static long id = 0;
    int start = 1;
    int i;
    struct thread_node *new_thread;
    // TODO: check invlaid argument
    if(low < 0 || low >= 360 || high < 0 || high >= 360 || (type != ROT_READ && type != ROT_WRITE)){
        return -EINVAL;
    }

    //(1) Struct dynamic alloc and init
    new_thread = (struct thread_node*)kmalloc(sizeof(struct thread_node), GFP_KERNEL);
    new_thread -> type = type;
    new_thread -> low = low;
    new_thread -> high = high;

    //(2) Check R/W locks & orientation, then set start
    read_lock(&orientation_lock);
    local_orientation = orientation;
    read_unlock(&orientation_lock);

    read_lock(&state_lock);
    if(is_degree_in_range(local_orientation, low, high))
    {
        if(low <= high)
        {
            for(i=low;i<=high;i++)
            {
                if((type == ROT_READ && access_state[i] == -1)||(type == ROT_WRITE && access_state[i] !=0))
                {
                    start = 0;
                    break;
                }
            }
        }
        else
        {
            for(i=high;i<=(low + 360);i++)
            {
                if((type == ROT_READ && access_state[(i%360)] == -1)||(type == ROT_WRITE && access_state[(i%360)] !=0))
                {
                    start = 0;
                    break;
                }
            }
        }
    }
    else
        start = 0;
    read_unlock(&state_lock);
    sema_init(&(new_thread -> start), start);

    //(3) add new_thread to linked list
    write_lock(&list_lock);
        new_thread -> id = id++;
        list_add(&(new_thread->list), &thread_list);
    write_unlock(&list_lock);

    down(&(new_thread->start));
    return id;
}

SYSCALL_DEFINE1(rotation_unlock, long, id){
    int local_orientation;
    int i;
    int low, high;
    struct thread_node* pos;

    // check invalid argument, return -EINVAL
    if (id < 0) return -EINVAL;

    read_lock(&orientation_lock);
    local_orientation = orientation;
    read_unlock(&orientation_lock);

    write_lock(&list_lock);
    //(1) Update R/W states corresponding to this thread
    // TODO: find thread_node corresponding to id, 
    list_for_each_entry(pos, &thread_list, list)
    {
        if (pos->id == id)
        {
            break;
        }
    }
    if (pos->id != id) return -EPERM;
    low = pos->low;
    high = pos->high;
    write_lock(&state_lock);

    if(low <= high)
    {
        for(i=low;i<=high;i++)
        {
            if(access_state[i] == -1)
                access_state[i] = 0;
            else if(access_state[i] > 0)
                access_state[i]--;
        }
    }
    else
    {
        for(i=high;i<=(low + 360);i++)
        {
            if(access_state[(i%360)] == -1)
                access_state[(i%360)] = 0;
            else if(access_state[(i%360)] > 0)
                access_state[(i%360)]--;
        }
    }
    /*
        TODO: will be done by Mr.Ju.
        (2)Check linked list and grant if possible
    */
    traverse_twice_give_rotlock();

    write_unlock(&state_lock);

    list_del(&(pos->list));
    kfree(pos);
    write_unlock(&list_lock);
    return 0;
}