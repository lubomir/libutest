#include <config.h>

#include "utils.h"

#include <stdarg.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

void
emit_message (const char *type, const char *msg, ...)
{
    va_list args;
    va_start(args, msg);
    fprintf(stderr, " *** %s: ", type);
    vfprintf(stderr, msg, args);
    fputc('\n', stderr);
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
    memset(mem, 0, size);
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
putc_color (char c, char *color)
{
    if (isatty(STDOUT_FILENO)) {
        printf("%s%c%s", color, c, NORMAL);
    } else {
        putchar(c);
    }
}

static void
write_strip_escapes (char *buffer, size_t len, FILE *dest)
{
    for (size_t i = 0; i < len; ++i) {
        if (buffer[i] == '\033' && buffer[i+1] == '[') {
            i += 2;
            while (buffer[i] < '@' || buffer[i] > '~') ++i;
            continue;
        }
        fputc(buffer[i], dest);
    }
}

void
copy_from_to (FILE *src, FILE *dest)
{
    static char buffer[4096];
    size_t len;
    bool tty = isatty(fileno(dest));

    while ((len = fread(buffer, 1, sizeof buffer, src)) > 0) {
        if (tty) {
            fwrite(buffer, len, 1, dest);
        } else {
            write_strip_escapes(buffer, len, dest);
        }
    }
}
