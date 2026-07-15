#include "lib/header.h"


void print_help(char *prog_name)
{
    printf("%s -h file                                       - помощь\n", prog_name);
    printf("%s [-q] file                                     - информация о времени файла     (-q - тихий режим)\n", prog_name);
    printf("%s -s [-R] [-q] file tmp_file                    - сохранить данные в файл        (-R - рекурсивно)\n", prog_name);
    printf("%s -r [-q] file tmp_file                         - загрузить информацию из файла  (-R - рекурсивно)\n", prog_name);
    printf("%s -t [-q] file Access_time Modify_time          - задать время файла вручную\n", prog_name);
    puts("time format: \"2026-07-12 15:43:59.017063875\"\n");

    puts("Примеры:");
    printf("%s -tq file \"2026-07-12 15:43:59.017063875\" \"2026-07-12 15:43:59.017063875\"\n", prog_name);
    printf("%s -t file \"2026-07-12 15:43:59.017063875\" -\n", prog_name);
    printf("%s -t file - \"2026-07-12 15:43:59.017063875\"\n", prog_name);
}

