#include "lib/header.h"

void write_to_file(char *path, struct database *db, int flag_info)
{
    ssize_t res_wr;
    int fd;
    
    fd = open(path, O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR /* 0600 */);         /* -rw------- */
    if(fd == -1)
        errExit("open");

    for (size_t i = 0; i < db->count; ++i) {
        ssize_t res;
        struct file_info *f = &db->files[i];
        
        int len = snprintf(
            NULL,
            0,
            "%s|%ju|%ju|%ld|%09ld|%ld|%09ld\n",
            f->path,
            (uintmax_t)f->inode,
            (uintmax_t)f->device,
            (long)f->atime.tv_sec,  (long)f->atime.tv_nsec,
            (long)f->mtime.tv_sec,  (long)f->mtime.tv_nsec);
        if (len < 0)
            errExit("snprintf");

        char *str_data = malloc(len + 1);
        if (str_data == NULL)
            errExit("malloc");

        snprintf(
            str_data,
            len + 1,
            "%s|%ju|%ju|%ld|%09ld|%ld|%09ld\n",
            f->path,
            (uintmax_t)f->inode,
            (uintmax_t)f->device,
            (long)f->atime.tv_sec,  (long)f->atime.tv_nsec,
            (long)f->mtime.tv_sec,  (long)f->mtime.tv_nsec);

        res = write(fd, str_data, len);
        if (res == -1)
            errExit("write");

        free(str_data);
    }

    if(flag_info)
        print_info(db);

    if(close(fd))
        errExit("close");
}