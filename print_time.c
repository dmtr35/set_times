#include "lib/header.h"


void print_time(const char *name, const struct timespec *ts)
{
    char buf[64];
    struct tm tm;

    localtime_r(&ts->tv_sec, &tm);

    strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", &tm);

    printf("%-15s %s.%09ld\n", name, buf, ts->tv_nsec);
}


void print_info(struct database *db)
{
    for (size_t i = 0; i < db->count; ++i) {
        printf("%-15s %s\n", "Path", db->files[i].path);
        printf("%-15s %ju\n", "I-node", (uintmax_t)db->files[i].inode);

        print_time("Access:", &db->files[i].atime);
        print_time("Modify:", &db->files[i].mtime);
        print_time("Change:", &db->files[i].ctime);

        puts("====================================================");
    }
}
