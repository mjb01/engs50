#include <stdio.h>
#include <pthread.h>
#include "locked_hash.h"

lockedhashtable_t *lhtp;
int value1 = 10;
int value2 = 20;

void *thread1(void *arg) {
    lhput(lhtp, &value1, "key1", 4);
    return NULL;
}

void *thread2(void *arg) {
    lhput(lhtp, &value2, "key2", 4);
    return NULL;
}

int main(void) {
    lhtp = lhopen(100);

    pthread_t tid1, tid2;
    pthread_create(&tid1, NULL, thread1, NULL);
    pthread_create(&tid2, NULL, thread2, NULL);

    pthread_join(tid1, NULL);
    pthread_join(tid2, NULL);

    int *result1 = (int *) lhget(lhtp, "key1", 4);
    int *result2 = (int *) lhget(lhtp, "key2", 4);
    printf("Value1: %d\n", *result1);
    printf("Value2: %d\n", *result2);

    lhclose(lhtp);
    return 0;
}