#include "lib/header.h"

void recursively(char *path, database *db)
{
    struct stat sb;
    DIR *info_dir;
    struct dirent *dirent;

    if(lstat(path, &sb) == -1)
        errExit("lstat");
    add_to_arr(path, &sb, db);

    if ((info_dir = opendir(path)) == NULL)
        errExit("opendir");
    
    
    while(dirent = readdir(info_dir)) {
        if(strcmp(dirent->d_name, ".") != 0 && strcmp(dirent->d_name, "..") != 0) {
            size_t size_path = strlen(path) + strlen(dirent->d_name) + 2;
            char *new_path = malloc(size_path);                                         // выделана память
            if (new_path == NULL)
                errExit("malloc");

            snprintf(new_path, size_path, "%s/%s", path, dirent->d_name);

            if(lstat(new_path, &sb) == -1)
                errExit("lstat");

            if (S_ISDIR(sb.st_mode)) {
                recursively(new_path, db);
            } else {
                add_to_arr(new_path, &sb, db);
            }
            free(new_path);
        }
    }
    closedir(info_dir);
}