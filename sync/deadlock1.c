#include <pthread.h>
#include <stdio.h>

pthread_t t1;
pthread_t t2;
pthread_mutex_t mlock1 = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mlock2 = PTHREAD_MUTEX_INITIALIZER;

void *thread_func1(void *args) {
    int cnt = 0;
    for(;;) {
        pthread_mutex_lock(&mlock1);
        pthread_mutex_lock(&mlock2);
        printf("Hey this is Thread 1, running in critical section. Run %d\n", cnt);
        cnt++;
        pthread_mutex_unlock(&mlock2);
        pthread_mutex_unlock(&mlock1);
    }
}

void *thread_func2(void *args) {
    int cnt = 0;
    for(;;) {
        pthread_mutex_lock(&mlock2);
        pthread_mutex_lock(&mlock1);
        printf("Hey this is Thread 2, running in critical section. Run %d\n", cnt);
        cnt++;
        pthread_mutex_unlock(&mlock1);
        pthread_mutex_unlock(&mlock2);
    }
}

int main(void) {
    pthread_create(&t1, NULL, thread_func1, NULL);
    pthread_create(&t2, NULL, thread_func2, NULL);
    while(1);
}