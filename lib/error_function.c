#include <stdarg.h>
#include "error_functions.h"
#include "header.h"

#ifdef __GNUC__
__attribute__ ((__noreturn__))
#endif

static void terminate(Boolean useExit3)
{
    char *s;
    /* Сохраняет дамп ядра, если переменная среды EF_DUMPCORE определена
    и содержит непустую строку; в противном случае вызывает exit(3)
    или _exit(2), в зависимости от значения 'useExit3'. */
    s = getenv("EF_DUMPCORE");
    if (s != NULL && *s != '\0')
        abort();
    else if (useExit3)
        exit(EXIT_FAILURE);
    else
        _exit(EXIT_FAILURE);
}

static void outputError(Boolean useErr, int err, Boolean flushStdout, const char *format, va_list ap)
{
#define BUF_SIZE 500
    char errText[BUF_SIZE];
    va_list ap_copy;
    va_copy(ap_copy, ap);
    size_t len_userMsg = vsnprintf(NULL, 0, format, ap_copy);

    char *userMsg = malloc(len_userMsg + 1);
    if (userMsg == NULL){
        perror("malloc userMsg");
        exit(EXIT_FAILURE);
    }

    vsnprintf(userMsg, len_userMsg + 1, format, ap);

    if (useErr)
        snprintf(errText, BUF_SIZE, " [%s]", strerror(err));
    else
        snprintf(errText, BUF_SIZE, ":");
    
    size_t len_buf = strlen("ERROR") + strlen(errText) + 1 + len_userMsg + 1 + 1;
    char *buf = malloc(len_buf);
    snprintf(buf, len_buf, "ERROR%s %s\n", errText, userMsg);
    
    if (flushStdout)
        fflush(stdout);         /* Сброс всего ожидающего стандартного вывода */
    fputs(buf, stderr);
    fflush(stderr);             /* При отсутствии построчной буферизации в stderr */
    va_end(ap_copy);
    free(userMsg);
    free(buf);
}

void errMsg(const char *format, ...)
{
    va_list argList;
    int savedErrno;
    savedErrno = errno;     /* В случае ее изменения на следующем участке */
    va_start(argList, format);
    outputError(TRUE, errno, TRUE, format, argList);
    va_end(argList);
    errno = savedErrno;
}

void errExit(const char *format, ...)
{
    va_list argList;
    va_start(argList, format);
    outputError(TRUE, errno, TRUE, format, argList);
    va_end(argList);
    terminate(TRUE);
}

void err_exit(const char *format, ...)
{
    va_list argList;
    va_start(argList, format);
    outputError(TRUE, errno, FALSE, format, argList);
    va_end(argList);
    terminate(FALSE);
}

void errExitEN(int errnum, const char *format, ...)
{
    va_list argList;
    va_start(argList, format);
    outputError(TRUE, errnum, TRUE, format, argList);
    
    terminate(TRUE);
}

void fatal(const char *format, ...)
{
    va_list argList;
    va_start(argList, format);
    outputError(FALSE, 0, TRUE, format, argList);
    va_end(argList);
    terminate(TRUE);
}


void usageErr(const char *format, ...)
{
    va_list argList;                                        /* все дополнительные аргументы падают в va_list */
    fflush(stdout);                                         /* Сброс всего ожидающего стандартного вывода */
    fprintf(stderr, "Usage: ");
    va_start(argList, format);                              /* va_start — инициализация. аргументы начинаются ПОСЛЕ format*/
    vfprintf(stderr, format, argList);                      /* vfprintf версия для va_list*/
    va_end(argList);                                        /* Освобождает внутреннее состояние. Всегда нужно вызывать после va_start */
    fflush(stderr);                                         /* При отсутствии построчной буферизации в stderr */
    exit(EXIT_FAILURE);
}

void cmdLineErr(const char *format, ...)
{
    va_list argList;
    fflush(stdout);                                         /* Сброс всего ожидающего стандартного вывода */
    fprintf(stderr, "Command-line usage error: ");
    va_start(argList, format);
    vfprintf(stderr, format, argList);
    va_end(argList);
    fflush(stderr);                                         /* При отсутствии построчной буферизации в stderr */
    exit(EXIT_FAILURE);
}

