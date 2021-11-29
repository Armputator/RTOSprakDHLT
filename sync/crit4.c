

#include <stdio.h>
#include <pthread.h>

/* Hinweis: man benötigt hier eigentlich kein volatile mehr (hier nur verwendet um die Änderungen
            zum vorherigen Beispiel gering zu halten) */
volatile int cnt_t1 = 0;
volatile int cnt_t2 = 0;
volatile int cnt = 0;

pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;

/* Um das Verriegelungsproblem saube zu lösen, muss das beanspruchen der Verriegleung eine
   "atomare" (unteilbare) Funktion sein. Dies geschieht jetzt mit Hilfe einer speziellen
   Variable, einem sog. "Mutex" (mutual exclusion). Die Mutex-Variable wird jetzt
   mit Unterstützung des Betriebssystems atomar getestet und gesetzt.
   Der POSIX-Standard stellt hier eine entsprechende Mutex-Api bereit.

   Wir stellen jetzt fest, dass es nicht mehr zu Abbweichungen der Zählerstände kommt.

   Ein weiterer Vorteil der Verwendung eines pthreas-mutex (abgesehen davon, dass es jetzt
   funktionier): Ist das Schloss gerade nicht verfügbar und es muss gewartet werden, so
   weiß dies das Betriebssystem jetzt und der Task wird in den "blocked"-Zustand versetzt.
   Blockiert also keine CPU-Zeit durch aktives Warten (busy-wait)
*/

void *count1(void *args) {
    for(;;) {
        pthread_mutex_lock(&lock);
        cnt_t1++;
        cnt++;
        pthread_mutex_unlock(&lock);
    }
}

void *count2(void *args) {
    for(;;) {
        pthread_mutex_lock(&lock);
        cnt_t2++;
        cnt++;
        pthread_mutex_unlock(&lock);
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