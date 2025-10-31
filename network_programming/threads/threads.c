#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <syscall.h>

void *thread_function(void *arg)
{
    int thread_num = *(int *)arg;
    pthread_t thread_handle = pthread_self();
    pid_t thread_id = gettid();

    printf("Thread %d: HANDLE = %lu, TID = %d\n", thread_num, thread_handle,
           thread_id);

    // Small delay to make output more interesting
    sleep(rand() % 3);

    printf("Thread %d (HANDLE %lu TID %d) exiting\n", thread_num, thread_handle,
           thread_id);
    return NULL;
}

int main()
{
    pthread_t thread1, thread2;
    int thread1_num = 1;
    int thread2_num = 2;
    int result;

    printf("Main thread starting...\n");

    // Create first thread
    result = pthread_create(&thread1, NULL, thread_function, &thread1_num);
    if (result != 0) {
        printf("Error creating thread 1\n");
        return 1;
    }

    // Create second thread
    result = pthread_create(&thread2, NULL, thread_function, &thread2_num);
    if (result != 0) {
        printf("Error creating thread 2\n");
        return 1;
    }

    // Wait for both threads to complete
    printf("Main thread waiting for threads to finish...\n");

    pthread_join(thread1, NULL);
    printf("Thread 1 joined\n");

    pthread_join(thread2, NULL);
    printf("Thread 2 joined\n");

    printf("Main exits\n");
    return 0;
}