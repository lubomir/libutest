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
 * Unlike malloc(3), this function initializes the memory to zero.
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

/**
 * Read all data from one file handle and write it to the other.
 *
 * If dest is not a TTY, all escape sequences will be stripped from the data.
 *
 * @param src   where to copy from
 * @param dest  where to write
 */
void copy_from_to(FILE *src, FILE *dest);

/**
 * Wrapper arount write(2). It writes count bytes from buf to file descriptor
 * fd. If the underlying function writes less bytes, it retries to write the
 * rest until all is written.
 *
 * @param fd    file descriptor to write to
 * @param buf   buffer to read data from
 * @param count number of bytes to write
 */
void safe_write(int fd, const void *buf, size_t count);

/**
 * Wrapper arount read(2). It reads count bytes to buf from file descriptor
 * fd. If the underlying function reads less bytes, it retries to read the
 * rest until all is read.
 *
 * @param fd    file descriptor to read from
 * @param buf   buffer to write data to
 * @param count number of bytes to read
 */
void safe_read(int fd, void *buf, size_t count);

#endif /* end of include guard: UTILS_H */
