#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <string.h>
#include "my_memmove.h"
#include "c-timer-lib-master/timer.h"

int main() {
    interval_t * a;
    create_interval(&a, "TEST", mono, ns);
    printf("Running '%s'\n", a->name);
    
    char src[] = "1234567890";
    char dest[] = "abcdefghij";

    start(a);
    MEMMOVE(dest, src+1, 9);
    MEMMOVE(dest, src+2, 1);
    stop(a);
    printf("RAW:\n START: %lld.%.9ld\n END: %lld.%.9ld\n", (long long) a->start.tv_sec, a->start.tv_nsec, (long long) a->stop.tv_sec, a->stop.tv_nsec);
    printf("OUT: %.9f %s\n", elapsed_interval(a, none), print_unit(a->unit));
    printf("%s\n", dest);    
    free(a);
    return 0;
}