#ifndef ERROR_FUNCTIONS_H
#define ERROR_FUNCTIONS_H

void errMsg(const char *format, ...);       /* Просто выводит сообщение об ошибке на stderr */
#ifdef __GNUC__
/* Этот макрос блокирует предупреждения компилятора при использовании
команды 'gcc -Wall', жалующиеся, что "control reaches end of non-void
function", то есть что управление достигло конца функции, которая
должна вернуть значение, если мы используем следующие функции для
прекращения выполнения main() или какой-нибудь другой функции,
которая должна вернуть значение определенного типа (не void) */

#define NORETURN __attribute__ ((__noreturn__))                 /* функция не возвращается */
#else
#define NORETURN
#endif


void errExit(const char *format, ...) NORETURN ;                        /* Просто выводит сообщение об ошибке на stderr */
void err_exit(const char *format, ...) NORETURN ;                       /* завершает программу, но делает это через _exit(). Полезно в дочерних процессах после fork() */
void errExitEN(int errnum, const char *format, ...) NORETURN ;          /* Как errExit, но можно явно указать код ошибки errnum, а не брать errno */
void fatal(const char *format, ...) NORETURN ;                          /* Просто выводит сообщение без errno и завершает программу. fatal не показывает errno*/
void usageErr(const char *format, ...) NORETURN ;                       /* Сообщение об неправильном использовании программы. Выводит текст "Usage: ..."*/
void cmdLineErr(const char *format, ...) NORETURN ;                     /* Сообщение об неправильном использовании программы */

#endif