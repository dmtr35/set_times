#include "lib/header.h"



int main(int argc, char *argv[])
{
    int res;
    struct stat sb;
    char path[MAX_PATH];
    char tmp_file[MAX_PATH];
    char atime[32] = {0};
    char mtime[32] = {0};
    int flag_save = 0, flag_settime = 0, flag_restor = 0, flag_info = 1, flag_recurs = 0;

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


    if (strchr(args, 'h') != NULL) {
        print_help(basename(argv[0]));
        return 0;
    }

    if (strchr(args, 'q') != NULL)
        flag_info = 0;
        
    for (char *p = args; *p; ++p) {
        if (*p == 'R')
            flag_recurs = 1;
        else if (*p == 't')
            flag_settime = 1;
        else if (*p == 's')
            flag_save = 1;
        else if (*p == 'r')
            flag_restor = 1;
    }

    strcpy(path, argv[first_non_option]);
    if (argc - first_non_option == 2) {
        strcpy(tmp_file, argv[first_non_option + 1]);
    } else if (argc - first_non_option == 3) {
        strcpy(atime, argv[first_non_option + 1]);
        strcpy(mtime, argv[first_non_option + 2]);
    }

// --------------------------------------------------
    char *full_path = realpath(path, NULL);                                             // выделяется память
    if (full_path == NULL)
        errExit("realpath");

    char *full_path_copy = strdup(full_path);                                           // выделяется память
    if (full_path_copy == NULL)
        errExit("strdup");

    char *parent_path = dirname(full_path_copy);            // обрезает full_path_copy (память не выделяется)
// --------------------------------------------------



     if (flag_settime) {
        set_time(full_path, atime, mtime, &db, flag_info);
    } else if (flag_save) {
        if(lstat(parent_path, &sb) == -1)
            errExit("lstat");
    
        add_to_arr(parent_path, &sb, &db);
        free(full_path_copy);
        
        if(lstat(full_path, &sb) == -1)
            errExit("lstat");
        
        if (S_ISDIR(sb.st_mode) && flag_recurs) {
            recursively(full_path, &db);
        } else {
            add_to_arr(full_path, &sb, &db);
            free(full_path);
        }

        write_to_file(tmp_file, &db, flag_info);

    } else if(flag_restor) {
        restor_meta(tmp_file, &db, flag_info);
    }


    free_database(&db);
                
    return 0;
}
