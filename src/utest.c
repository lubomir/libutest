#include "utest.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <unistd.h>

typedef void (*func_t)(void);

typedef struct {
    size_t size;
    size_t num;
    const char const **names;
    func_t *funcs;
    func_t setup;
    func_t teardown;
} tests_t;

static tests_t *tests = NULL;
static const char *current_test_name = NULL;
static unsigned int tests_failed = 0;
static unsigned int assertions_ok = 0;
static unsigned int assertions_failed = 0;
static FILE * logs = NULL;

void shutdown_tests(void)
{
    free(tests->funcs);
    free(tests->names);
    free(tests);
}

static inline
void * safe_malloc (size_t size)
{
    void *mem = malloc(size);
    if (!mem) {
        fprintf(stderr, " *** ERROR: memory allocation failed\n");
        abort();
    }
    return mem;
}

static inline
void * safe_realloc (void *mem, size_t size)
{
    void *new_mem = realloc(mem, size);
    if (!new_mem) {
        fprintf(stderr, " *** ERROR: memory allocation failed\n");
        free(mem);
        abort();
    }
    return new_mem;
}

void init_tests(void)
{
    tests = malloc(sizeof *tests);
    tests->size = 8;
    tests->num = 0;
    tests->funcs = safe_malloc(sizeof(func_t) * tests->size);
    tests->names = safe_malloc(sizeof(func_t) * tests->size);
    tests->setup = tests->teardown = NULL;
    atexit(shutdown_tests);
}

void ut_register_test(const char const *name, func_t f)
{
    if (tests == NULL)
        init_tests();
    if (tests->num >= tests->size) {
        tests->size = 2 * tests->size;
        tests->funcs = safe_realloc(tests->funcs, sizeof(func_t) * tests->size);
        tests->names = safe_realloc(tests->names, sizeof(func_t) * tests->size);
    }
    tests->names[tests->num] = name;
    tests->funcs[tests->num] = f;
    tests->num++;
}

void ut_register_callback(void (*cb)(void), int type)
{
    if (tests == NULL)
        init_tests();
    switch (type) {
    case 0:
        tests->setup = cb;
        break;
    case 1:
        tests->teardown = cb;
        break;
    }
}

#define BUFFER_SIZE 256

int ut_run_all_tests(void)
{
    logs = tmpfile();

    for (size_t i = 0; i < tests->num; i++) {
        unsigned int old_fails = assertions_failed;
#ifdef DEBUG
        fprintf(stderr, "Running test '%s'\n", tests->names[i]);
#endif
        if (tests->setup) {
            tests->setup();
        }
        current_test_name = tests->names[i];
        tests->funcs[i]();
        if (tests->teardown) {
            tests->teardown();
        }
        if (old_fails != assertions_failed) {
            tests_failed++;
            printf(RED "F" NORMAL);
        } else {
            printf(GREEN "." NORMAL);
        }
    }
    printf("\n\n");
    char buffer[BUFFER_SIZE];
    rewind(logs);
    while (fgets(buffer, BUFFER_SIZE, logs) != NULL) {
        printf("%s", buffer);
    }

    printf("%u assertions succeeded, %u assertions failed\n",
            assertions_ok, assertions_failed);
    printf("%zu tests ran\n", tests->num);
    fclose(logs);
    return tests_failed == 0;
}

void ut_assert_func(const char *file,
                    int lineno,
                    int expr,
                    const char *msg, ...)
{
    if (expr) {
        assertions_ok++;
        return;
    }
    assertions_failed++;
    fprintf(logs, "Assertion in %s%s%s (%s:%d) failed:\n\t",
            BOLD, current_test_name, NORMAL, file, lineno);
    va_list args;
    va_start(args, msg);
    vfprintf(logs, msg, args);
    va_end(args);
    fprintf(logs, "\n\n");
}
