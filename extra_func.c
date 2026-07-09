#include "lib/header.h"

void set_timespec(void *ts, char *str_data, int flag_type)
{
    char *ptr;
    if((ptr = strrchr(str_data, '|')) != NULL) {
        if(flag_type == 0)
            *(time_t *)ts = strtol(&ptr[1], NULL, 10);
        else if (flag_type == 1)
            *(long *)ts = strtol(&ptr[1], NULL, 10);
        else if (flag_type == 2)
            *(dev_t *)ts = strtol(&ptr[1], NULL, 10);
        else if (flag_type == 3)
            *(ino_t *)ts = strtol(&ptr[1], NULL, 10);
        ptr[0] = '\0';
    }
}


void set_time(char *path_file, struct database *db,  a_m_time amt, int flag_info)
{
    struct stat sb;
    struct timespec times[2];
    int fd;

    char *str = "2024-07-06 16:30:13";

    struct tm tm = {0};
    if (strptime(str, "%Y-%m-%d %H:%M:%S", &tm) == NULL) {
        printf("Ошибка разбора\n");
        return;
    }

    time_t t = mktime(&tm);

    printf("%ld\n", (long)t);
}

// split_time()
// {
//     printf("gg");
// }

void free_database(struct database *db)
{
    for(int i = 0; i < db->count; ++i) {
        free(db->files[i].path);
    }
    free(db->files);

    db->files = NULL;
    db->count = 0;
    db->capacity = 0;
}