#include "lib/header.h"

void restor_meta(char *path_file, struct database *db, int flag_info)
{
    struct stat sb; 
    struct timespec times[2];
    int fd, offset = 0, count = 0, n = 256;
    ino_t saved_inode;
    dev_t saved_dev;
    ssize_t res;
    char buffer[MAX_PATH + 1] = {0};
    
    fd = open(path_file, O_RDONLY);
    if(fd == -1)
    errExit("open");
    
    while(res = read(fd, buffer, n)) {
        if((strchr(buffer, '\n')) == NULL) {
            n *= 2;
            if(lseek(fd, 0, SEEK_SET) == -1)
            errExit("lseek");
            if(n > LIMIT)
                fatal("limit memory");
            } else {
                char *ptr;
                size_t end = strcspn(buffer, "\n");
            char *str_data = malloc(end + 1);
            if(str_data == NULL)
            errExit("malloc");
            
            strncpy(str_data, buffer, end);
            str_data[end] = '\0';
            
            // ===========================================================
            set_timespec(&times[1].tv_nsec, str_data, 1);
            set_timespec(&times[1].tv_sec, str_data, 0);
            set_timespec(&times[0].tv_nsec, str_data, 1);
            set_timespec(&times[0].tv_sec, str_data, 0);
            set_timespec(&saved_dev, str_data, 2);
            set_timespec(&saved_inode, str_data, 3);
            
            if(lstat(str_data, &sb) == -1)
                errExit("lstat");
            
            
            if (sb.st_ino == saved_inode && sb.st_dev == saved_dev)
                if (utimensat(AT_FDCWD, str_data, times, 0) == -1)
                    errExit("utimensat");
            
            if(lstat(str_data, &sb) == -1)
                errExit("lstat");
            
            add_to_arr(str_data, &sb, db);
            // ===========================================================
            free(str_data);
            
            offset += end + 1;
            if(lseek(fd, offset, SEEK_SET) == -1)
            errExit("lseek");
        }
    }
    
    if(flag_info)
        print_info(db);  
}