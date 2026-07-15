#define _GNU_SOURCE                             /* realpath */
#include <limits.h>                             /* realpath */
#include <stdlib.h>                             /* realpath */
#include <libgen.h>                             /* dirname, basename */
#include <stdio.h>                              /* printf, sprintf, snprintf */
#include <sys/stat.h>                           /* stat, lstat, fstat, utimensat */
#include <sys/types.h>                          /* ino_t, dev_t, mode_t, uid_t, gid_t, off_t */
#include <time.h>                               /* strftime */
#include <string.h>                             /* strstr, strcmp, strdup */
#include <dirent.h>                             /* opendir, readdir */

#include <fcntl.h>                              /* open, O_WRONLY, O_CREAT, O_TRUNC, AT_FDCWD */
#include <unistd.h>                             /* read, write */
#include <sys/stat.h>                           /* open: "S_IRUSR, S_IWUSR" */
#include <inttypes.h>                           /* uintmax_t */

#include <errno.h>                              /* Объявление errno и определение констант ошибок */
#include "../lib/error_functions.h"

typedef enum { FALSE, TRUE } Boolean;
#define min(m,n) ((m) < (n) ? (m) : (n))
#define max(m,n) ((m) > (n) ? (m) : (n))

#define MAX_PATH 256
#define LIMIT 4096

typedef struct file_info {
    ino_t inode;
    dev_t device;
    mode_t mode;
    uid_t uid;
    gid_t gid;
    off_t size;
    struct timespec atime;
    struct timespec mtime;
    struct timespec ctime;
    char *path;
} file_info;

typedef struct database {
    struct file_info *files;
    size_t count;
    size_t capacity;
} database;

typedef struct a_m_time {
    time_t time_s;
    long time_ms;
} a_m_time;


//  checkers.c
int check_timespec(struct timespec*, struct timespec*);

// extra_func.c
void set_timespec(void*, char*, int);
void set_time(char*, char*, char*, struct database*, int);
void split_time(char*, struct timespec*);
void add_to_arr(char*, struct stat*, database*);
void free_database(struct database*);

// print_help.c
void print_help(char*);

// print_time.c
void print_time(const char*, const struct timespec*);
void print_info(struct database*);

// recursion.c
void recursively(char*, database*);

// restor_meta.c
void restor_meta(char*, struct database*, int);

// write_time.c
void write_to_file(char*, struct database*, int);