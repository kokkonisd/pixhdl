#ifndef __dbg_h__
#define __dbg_h__

#include <stdio.h>
#include <errno.h>
#include <string.h>

#ifdef NDEBUG
#define debug(M, ...)
#else
#define debug(M, ...) fprintf(stderr, "DEBUG %s:%s():%d: " M "\n",\
        __FILE__, __FUNCTION__, __LINE__, ##__VA_ARGS__)
#endif

#define clean_errno() (errno == 0 ? "None" : strerror(errno))

#ifdef NDEBUG
#define log_err(M, ...) fprintf(stderr, \
        "\e[3;31m[pixhdl]\e[0m\e[31m " M "\e[0m\n", ##__VA_ARGS__)
#else
#define log_err(M, ...) fprintf(stderr, \
        "[ERROR] (%s:%s():%d: errno: %s) " M "\n", __FILE__, __FUNCTION__, \
        __LINE__, clean_errno(), ##__VA_ARGS__)
#endif

#define log_warn(M, ...) fprintf(stderr, \
        "[WARN] (%s:%s():%d: errno: %s) " M "\n",\
        __FILE__, __FUNCTION__, __LINE__, clean_errno(), ##__VA_ARGS__)

#define log_info(M, ...) fprintf(stderr, "\e[3;33m[pixhdl]\e[0m\e[33m " M \
                                         "\e[0m\n", ##__VA_ARGS__)

#define check(A, M, ...) if (!(A)) {\
        log_err(M, ##__VA_ARGS__); errno = 0; goto error; }

#define sentinel(M, ...) { log_err(M, ##__VA_ARGS__); \
        errno = 0; goto error; }

#define check_mem(A) check((A), "Out of memory.")

#define check_debug(A, M, ...) if (!(A)) { debug(M, ##__VA_ARGS__); \
        errno = 0; goto error; }

#endif
