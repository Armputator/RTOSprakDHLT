/* Mit Threads */

#include <stdio.h>
#include <pthread.h>

volatile int cnt_t1 = 0;
volatile int cnt_t2 = 0;
volatile int cnt = 0;

/* Jetzt werden die Funktionen count1 u. count2 nicht mehr explizit im Hauptprogramm aufgerufen, sondern
   sie werden als eigene Threads (mit gleicher Priorität) realisiert.
   Die Threads erhalten eigenen Endlosschleifen. Sie werden einmal gestartet und werden erst beendet wenn
   das Hauptprogramm beendet wird.
   Auch hier würden wir eigentlich erwarten, dass die Summe aus cnt_t1 u. cnt_t2 immer dem Zählerstand von cnt
   entspricht, oder zumindest eine Differenz von max. 1, wenn wir "gerade im falschen Moment hinschauen".
   Allerdings stellen wir hier jetzt fest, dass der Zähler nicht korrekt zählt.

   Frage an Sie: warum ist das so? Was passiert hier? (Antwort unten)
   */

void *count1(void *args) {
    for(;;) {
        cnt_t1++;
        cnt++;
    }
}

void *count2(void *args) {
    for(;;) {
        cnt_t2++;
        cnt++;
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

/*  Das Inkrementieren von cnt mit cnt++ wird im Maschinencode auf mehrere Maschineninstruktionen abgebildet
    (lesen, modifizieren, schreiben). Es kann jederzeit passieren, dass der Scheduler den Programmablauf
    innerhalb der ++-Routine (z.B in count1) unterbricht und dann dann mit count2 weitermacht. Wenn wir lange
    genug warten wird irgendwann folgende Situation eintreten:
        - Innerhalb von count1 wirde gerade "cnt++" asugeführt. Der Scheduler unterbricht jetzt den
          Programmablauf nach dem Lesen des aktuellen Zählerstandes und übergibt die CPU an count2.
        - In count2 wird jetzt ebenfalls der (noch unveränderte) Zählerstand von cnt gelesen, inkrementiert
          und geschrieben.
        - Die CPU geht (später) wieder an count1. Dort geht es jetzt mit dem Inkrement weiter, allerdings
          auf Basis eines alten Wertes, nämlich dem gleichen der auch schon in count2 für das Inkrement
          verwendet wurde.
    Es findet also zwei mal das gleich Inkrement statt und ein Zählvorgang wird "verschluckt" . Da der Code
    sehr kurz ist und sehr häufig ausgeführt wird in diesem Beispiel tritt diese Situation sehr schnell
    und häufig auf. Dies ist in der Realität oft nicht so, was solche Fehler sehr gefährlich macht, da sie
    u.U. erst spät im Feld das erste Mal auftreten und fatale Folgen haben.

    Frage an Sie: Was kann man dagegen tun?
*/