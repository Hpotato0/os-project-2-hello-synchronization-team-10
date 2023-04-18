# Project 2: hello, synchroization!
* team 10: 김현석, 홍주원, 주재형
* main implementation: ./kernel/rotation.c
* test code: ./test/{rotd.c,professor.c,student.c}

## 0. Running & Testing
Nothing changed from project0 README.

## 1. Implementation Overview
If at some point the condition for an access request to be granted is satisfied, it is granted.

This is guaranteed by keeping a linked list of all access requests and traversing it every time the related states change
i.e. every time (1)the orientation changes, or (2)a granted lock gets revoked.

To keep write from starving, the traversal actually happens twice. The first to give all possible write grants, and the second to give all possible read grants.

Even when a lock request is granted, the corresponding linked list node doesn't get deleted, but gets marked with 'is_started' so that it is skipped during the traversal.

A node gets deleted when (1)the granted access gets normally revoked, or (2)the corresponding thread exits-if access was granted but not revoked, it is auto revoked during exit.

The 'auto' revocation is done by the funciton exit_rotlock, which is called inside do_exit() in kernel/exit.c.

One global variable(orientation) and two global structures(thread_list, access state) are used to keep track of all needed info. A read-write lock is created for each of these global objects, so a total of three read-write locks.

## 2. Global Structures
**thread_list**: a linked list saving all access requests, both granted and not granted
* **struct list_head list**  : 'inherits' the linked list
* **int type, low, high**    : basic info
* **struct semaphore start** : the actual lock that blocks in rotation_lock until access is granted
* **long id**                : the id that rotation_lock returns, and rotation_unlock accepts
* **int is_started**         : marks granted requests
* **pid_t pid**              : for auto revocation when a thread exits without revoking

**access_state**: an int array of 360, saves the access state per angle
* 0        : FREE
* -1       : a thread is WRITING
* n (n>0)  : n threads are READING

## 3. Lessons Learned
The linux kernel does OOC using C! (struct list_head)