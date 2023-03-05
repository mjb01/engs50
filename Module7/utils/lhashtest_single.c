#include <stdio.h>
#include "locked_hash.h"

int main(void) {
    lockedhashtable_t *lhtp = lhopen(100);

    int value1 = 10;
    lhput(lhtp, &value1, "key1", 4);

    int *result = (int *) lhget(lhtp, "key1", 4);
    printf("Value: %d\n", *result);

    lhclose(lhtp);
    return 0;
}
