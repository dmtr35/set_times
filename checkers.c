#include "lib/header.h"

int check_timespec(struct timespec *saved, struct timespec *current)
{
    if (saved->tv_nsec == UTIME_OMIT)
        return 1;

    return saved->tv_sec == current->tv_sec &&
           saved->tv_nsec == current->tv_nsec;
}
