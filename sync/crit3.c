/* Mit manuellem Lock */

#include <stdio.h>
#include <pthread.h>
#include <stdatomic.h>

/* Achtung volatile wird grundsätzlich nicht benötigt wenn man mit threads arbeitet */
volatile int cnt_t1 = 0;
volatile int cnt_t2 = 0;
volatile int cnt = 0;

volatile atomic_int lock=0;

void *count1(void *args) {
    for(;;) {
        while(lock);
        lock = 1;
        cnt_t1++;
        cnt++;
        lock = 0;
    }
}

void *count2(void *args) {
    for(;;) {
        while(lock);
        lock = 1;
        cnt_t2++;
        cnt++;
        lock = 0;
    }
}

int main(void) {
    pthread_t t1, t2;
    pthread_create(&t1, NULL, count1, NULL);
    pthread_create(&t2, NULL, count2, NULL);
    int sum;
    while(1) {
        //count1(NULL);
        //count2(NULL);
        sum = cnt_t1+cnt_t2;
        printf("%d:%d:%d\n",cnt,sum,cnt-sum);
    }
}