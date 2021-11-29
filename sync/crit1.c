#include <stdio.h>

volatile int cnt_t1 = 0;
volatile int cnt_t2 = 0;
volatile int cnt = 0;

/* In diesem Beispiel wird in zwei Unterprogrammen jeweils ein gemeinsamer Zähler inkrementiert und
   noch ein weiterer Zähler der nur exklusiv in der jeweiligen Funktion inkrmentiert wird.
   Wir würden also erwarten, dass nach jedem Funktionsaufruf der gemeinsame Zähler der Summe der beiden
   exklusiven Zähler entspricht. Also cnt == cnt_t1 + cnt_t2.
   Wenn wir das Programm kompilieren und starten, stellen wir auch fest, dass dies der Fall ist.
   Dieses Programm hat nur einen Thread, die Funktionen count1 u. count2 werden streng nacheinander ausgeführt.
   (Natürlich kann und wird es passieren, dass der Scheduler die Ausführung einer der beiden Funktionen an beliebiger Stelle unterbricht
   und ein ganz anderer Task, der nichts mit diesem Programm zu tun hat weiter macht. Es kann hier allerdings nicht passieren,
   dass z.B. count1 unterbrochen wird und es dann mit count2 weitergeht.) */

void *count1(void *args) {
    cnt_t1++;
    cnt++;
}

void *count2(void *args) {
    cnt_t2++;
    cnt++;
}

int main(void) {
    int sum;
    while(1) {
        count1(NULL);
        count2(NULL);
        sum = cnt_t1+cnt_t2;
        printf("%d:%d:%d\n",cnt,sum,cnt-sum);
    }
}