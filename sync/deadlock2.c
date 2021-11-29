#include <pthread.h>
#include <stdio.h>

pthread_t t1;
pthread_t t2;
pthread_mutex_t mlock1 = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mlock2 = PTHREAD_MUTEX_INITIALIZER;

/* Abhilfe schafft, die Reihenfolge der Verriegelungen zu ändern. Wir schaffen dadurch zwei kritische
   Bereiche, sodass lock2 nie beansprucht werden kann wenn nicht auch lock1 beansprucht.
   Unten verdeutlicht durch zusätzliche Einrückungen im Code */

void *thread_func1(void *args) {
    int cnt = 0;
    for(;;) {
        pthread_mutex_lock(&mlock1); // Beginn äußerer kritischer Bereich
          pthread_mutex_lock(&mlock2); // Beginn innerer kritischer Bereich
            printf("Hey this is Thread 1, running in critical section. Run %d\n", cnt);
            cnt++;
          pthread_mutex_unlock(&mlock2); // Ende innerer kritischer Bereich
        pthread_mutex_unlock(&mlock1); // Ende äußerer kritischer Bereich
    }
}

void *thread_func2(void *args) {
    int cnt = 0;
    for(;;) {
        pthread_mutex_lock(&mlock1); // Beginn äußerer kritischer Bereich
          pthread_mutex_lock(&mlock2); // Beginn innerer kritischer Bereich
            printf("Hey this is Thread 2, running in critical section. Run %d\n", cnt);
            cnt++;
          pthread_mutex_unlock(&mlock2); // Ende innerer kritischer Bereich
        pthread_mutex_unlock(&mlock1); // Ende äußerer kritischer Bereich
    }
}

int main(void) {
    pthread_create(&t1, NULL, thread_func1, NULL);
    pthread_create(&t2, NULL, thread_func2, NULL);
    while(1);
}