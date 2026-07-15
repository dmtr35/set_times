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


void set_time(char *path_file, char *a_time, char *m_time, struct database *db, int flag_info)
{
    struct timespec times[2];
    struct stat sb;
    int res = 0;
    
    split_time(a_time, &times[0]);
    split_time(m_time, &times[1]);
    

    if (utimensat(AT_FDCWD, path_file, times, 0) == -1)
        errExit("utimensat");


    if(lstat(path_file, &sb) == -1)
        errExit("lstat");

    add_to_arr(path_file, &sb, db);
    
    if (!check_timespec(&times[0], &sb.st_atim))
        printf("Access time was not restored exactly\n");
    if (!check_timespec(&times[1], &sb.st_mtim))
        printf("Modify time was not restored exactly\n");

    if(flag_info)
        print_info(db); 
}

void split_time(char *timestamp, struct timespec *time)
{
    struct tm tm = {0};

    memset(time, 0, sizeof(*time));

    if (strlen(timestamp) == 1 && strchr(timestamp, '-') != NULL) {
        time->tv_nsec = UTIME_OMIT;
        return;
    }

    char *dot = strchr(timestamp, '.');
    if (dot != NULL)
        *dot = '\0';

    if (strptime(timestamp, "%Y-%m-%d %H:%M:%S", &tm) == NULL) {
        err_exit("Ошибка разбора времени");
    }
    
    time->tv_sec = mktime(&tm);
    time->tv_nsec = (long) strtol(dot + 1, NULL, 10);
}


void add_to_arr(char *path, struct stat *sb, database *db)
{
    if(db->capacity == db->count) {
        db->capacity *= 2;
        
        struct file_info *tmp = realloc(db->files, db->capacity * sizeof(struct file_info));

        if (tmp == NULL)
            errExit("realloc");

        db->files = tmp;
    }

    db->files[db->count].path = strdup(path);
    if (db->files[db->count].path == NULL)
        errExit("strdup");

    db->files[db->count].atime = sb->st_atim;
    db->files[db->count].ctime = sb->st_ctim;
    db->files[db->count].mtime = sb->st_mtim;
    db->files[db->count].inode = sb->st_ino;
    db->files[db->count].device = sb->st_dev;
    db->files[db->count].uid = sb->st_uid;
    db->files[db->count].gid = sb->st_gid;
    db->files[db->count].mode = sb->st_mode;
    db->files[db->count].size = sb->st_size;

    (db->count)++;
}


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