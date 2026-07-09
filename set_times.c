#include "lib/header.h"


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


void func_tree(char *path, database *db)
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
                func_tree(new_path, db);
            } else {
                add_to_arr(new_path, &sb, db);
            }
            free(new_path);
        }
    }
    closedir(info_dir);
}


int main(int argc, char *argv[])
{
    int res;
    struct stat sb;
    char path[MAX_PATH];
    char path_file[MAX_PATH];
    char atime[32] = {0};
    char mtime[32] = {0};
    int flag_save = 0, flag_restor = 0, flag_settime, flag_info = 1;

    database db = {};
    db.count = 0;
    db.capacity = 2;
    db.files = malloc(db.capacity * sizeof(struct file_info));
    if (db.files == NULL)
        errExit("malloc");

    
    // ----------------------------------------------------------
    char args[32] = "";
    size_t j = 0;
    int first_non_option = 1;

        for (; first_non_option < argc; ++first_non_option) {

        if (strcmp(argv[first_non_option], "--") == 0) {
            ++first_non_option;
            break;
        }

        if (argv[first_non_option][0] != '-')
            break;
        
        for (char *p = argv[first_non_option] + 1; *p; ++p)
            args[j++] = *p;
    }

    args[j] = '\0';
    // ----------------------------------------------------------


    for (char *p = args; *p; ++p) {
        if (*p == 'q')
            flag_info = 0;
        else if (*p == 't')
            flag_settime = 1;
        else if (*p == 's')
            flag_save = 1;
        else if (*p == 'r')
            flag_restor = 1;
    }

    strcpy(path, argv[first_non_option]);
    if (argc - first_non_option == 2) {
        strcpy(path_file, argv[first_non_option + 1]);
    } else if (argc - first_non_option == 3) {
        strcpy(atime, argv[first_non_option + 1]);
        strcpy(mtime, argv[first_non_option + 2]);
    }

    if (flag_settime) {

        char *end_atime = strchr(atime, '.');
        char *atime_ms = strchr(atime, '.') + 1;
        char *end_mtime = strchr(mtime, '.');
        char *mtime_ms = strchr(mtime, '.') + 1;
        *end_atime = '\0';
        *end_mtime = '\0';
        a_m_time amt = {atime, atime_ms, mtime, mtime_ms};
        set_time(path_file, &db, amt, flag_info);
        return 0;
    } else if(flag_restor) {
        restor_meta(path_file, &db, flag_info);
        return 0;
    }

    char *full_path = realpath(path, NULL);                                             // выделяется память
    if (full_path == NULL)
        errExit("realpath");

    char *full_path_copy = strdup(full_path);                                           // выделяется память
    if (full_path_copy == NULL)
        errExit("strdup");

    char *parent_path = dirname(full_path_copy);            // обрезает full_path_copy (память не выделяется)


    if(lstat(parent_path, &sb) == -1)
        errExit("lstat");

    add_to_arr(parent_path, &sb, &db);
    free(full_path_copy);
    
    if(lstat(full_path, &sb) == -1)
        errExit("lstat");
    
    if (S_ISDIR(sb.st_mode)) {
        func_tree(full_path, &db);
    } else {
        add_to_arr(full_path, &sb, &db);
    }
    
    free(full_path);
          
    write_to_file(path_file, &db, flag_info);

    free_database(&db);
                
    return 0;
}