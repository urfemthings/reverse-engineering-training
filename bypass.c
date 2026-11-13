#define _GNU_SOURCE
#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include <dlfcn.h>

FILE *fopen(const char *path, const char *mode) {
    static FILE *(*real_fopen)(const char*, const char*) = NULL;
    if (!real_fopen) real_fopen = dlsym((void*)-1, "fopen");
    if (strstr(path, "status")) {
        FILE *f = tmpfile();
        fprintf(f, "Name:\tlatihan9\nTracerPid:\t0\n");
        rewind(f);
        return f;
    }
    return real_fopen(path, mode);
}
