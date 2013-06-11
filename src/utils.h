#ifndef UTILS_H
#define UTILS_H

#include <stdio.h>

#define RED     "\033[1;31m"
#define GREEN   "\033[1;32m"
#define BOLD    "\033[1m"
#define NORMAL  "\033[0m"

/**
 * Print error message, printf-like format.
 */
#define error(...)     emit_message("ERROR", __VA_ARGS__)
/**
 * Print warning message, printf-like format.
 */
#define warning(...)   emit_message("WARNING", __VA_ARGS__)

/**
 * Print debug message, printf-like format. Only if DEBUG was defined at
 * compile time.
 */
#ifdef DEBUG
# define debug(...)    emit_message("DEBUG", __VA_ARGS__)
#else
# define debug(...)
#endif

/**
 * Print message to standard error output.
 *
 * @param type  string describing type of the message
 * @param msg   error message format string
 * @param ...   printf-like arguments
 */
void emit_message(const char *type, const char *msg, ...)
    __attribute__((format(printf, 2, 3)));

/**
 * Allocate a chunk of memory. This function is similar to malloc, only when
 * allocation fails, it prints error message and exits the program immediately.
 *
 * @param size  size of the memory block
 * @return pointer to newly allocated memory
 */
void * safe_malloc (size_t size)             __attribute__((alloc_size(1)));

/**
 * Safe version of realloc. If allocation fails, the original block is freed,
 * error message printed and whole program exited immediately.
 *
 * @param mem   memory block to be resized
 * @param size  size of the new block
 * @return pointer to resized memory area
 */
void * safe_realloc (void *mem, size_t size) __attribute__((alloc_size(2)));

/**
 * Print a character on stdout with given color. If stdout is not a terminal,
 * the color will be skipped.
 *
 * @param c     char to be printed
 * @param color ANSI color escape sequence
 */
void putc_color(char c, char *color);

#endif /* end of include guard: UTILS_H */
