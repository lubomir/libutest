#include <config.h>

#include "utils.h"

#include <stdarg.h>
#include <stdlib.h>
#include <unistd.h>

void emit_message(const char *type, const char *msg, ...)
{
    va_list args;
    va_start(args, msg);
    fprintf(stderr, " *** %s: ", type);
    vfprintf(stderr, msg, args);
    va_end(args);
}

void *
safe_malloc (size_t size)
{
    void *mem = malloc(size);
    if (!mem) {
        error("memory allocation failed");
        exit(EXIT_FAILURE);
    }
    return mem;
}

void *
safe_realloc (void *mem, size_t size)
{
    void *new_mem = realloc(mem, size);
    if (!new_mem) {
        error("memory allocation failed");
        free(mem);
        exit(EXIT_FAILURE);
    }
    return new_mem;
}

void
putc_color(char c, char *color)
{
    if (isatty(STDOUT_FILENO)) {
        printf("%s%c%s", color, c, NORMAL);
    } else {
        putchar(c);
    }
}

void
copy_from_to(FILE *src, FILE *dest)
{
    static char buffer[4096];
    size_t len;
    while ((len = fread(buffer, 1, sizeof buffer, src)) > 0) {
        fwrite(buffer, len, 1, dest);
    }
}
