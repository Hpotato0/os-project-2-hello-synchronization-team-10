# Project 2: hello, synchronization!
* team 10: 김현석, 홍주원, 주재형
* main implementation: ./kernel/rotation.c
* test code: ./test/{rotd.c,professor.c,student.c}

## 0. Running & Testing
The kernel build, test code compilation & running procedure did not change from project0 README. Use tmux to conveniently run and monitor multiple binaries.
```bash
root:~> ./rotd
```
```bash
root:~> ./professor 2748
```
```bash
root:~> ./student 10 70
```

## 1. Implementation Overview
Three syscalls are implemented: set_orientation, rotation_lock, rotation_unlock.

If at some point the condition for a lock request to be granted is satisfied, it is granted.

This is guaranteed by keeping a linked list of all requests and traversing it every time the related states change
i.e. every time (1)the orientation changes(set_orientation), or (2)a granted lock gets revoked(rotation_unlock). Of course, the lock can also be directly granted at rotation_lock, when it is first requested.

To keep write requests from starving, the traversal actually happens twice. The first to give all possible write grants, and the second to give all possible read grants.

Even when a lock request is granted, the corresponding linked list node doesn't get deleted, but gets marked with 'is_started' so that it is skipped during the traversal.

A node gets deleted when (1)the granted lock gets normally revoked, or (2)the corresponding thread exits-if a lock was granted but not revoked, it is auto revoked during exit.

The 'auto' revocation is done by the funciton exit_rotlock, which is called inside do_exit() in kernel/exit.c.

One global variable(orientation) and two global structures(thread_list, access state) are used to keep track of all needed info. A read-write lock is created for each of these global objects, so a total of three read-write locks.

## 2. Global Structures
**thread_list**: A linked list saving all lock requests(both granted and not granted). Each node is a `struct thread_node` and consists of:
* **struct list_head list**  : 'inherits' the linked list
* **int type, low, high**    : basic info
* **struct semaphore start** : the actual lock that blocks in rotation_lock until access is granted
* **long id**                : the id that rotation_lock returns, and rotation_unlock accepts
* **int is_started**         : marks granted requests
* **pid_t pid**              : for auto revocation when a thread exits without revoking

**access_state**: An int array of 360 elements. Saves the access state per angle in the following format:
* 0        : FREE
* -1       : a thread is WRITING
* n (n>0)  : n threads are READING

## 3. Detailed Implementation: Access Grants
The semaphore `start` of each request is initialized at `rotation_lock` to either 1 or 0, based on whether the lock could be granted or not. Then at the end of `rotation_lock`, `down` to the semaphore is called. If `new_thread->start` was initialized to 0 this would block until `up` is called during the traversals done in `rotation_unlock` and `set_orientation`.
```c
down(&(new_thread->start));
return new_thread -> id;
```

```c
up(&pos->start);
```

## 4. Lessons Learned
* The linux kernel does OOC using C! (struct list_head)
