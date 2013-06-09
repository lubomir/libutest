#include "utest.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <unistd.h>

#ifdef DEBUG
# define debug(...) fprintf(stderr, " *** DEBUG: " __VA_ARGS__);
#else
# define debug(...)
#endif

typedef struct suite Suite;

struct suite {
    const char *name;
    size_t size;
    size_t num;
    const char ** names;
    func_t *funcs;
    cb_t setup;
    cb_t teardown;
    struct suite *next;
};

struct test_data {
    const char *name;
    unsigned int assertions_ok;
    unsigned int assertions_failed;
    FILE *logs;
};

static Suite *tests = NULL;

static void
shutdown_tests (void)
{
    debug("Shutting down tests\n");
    Suite *s = tests;
    while (s) {
        tests = s->next;
        free(s->funcs);
        free(s->names);
        free(s);
        s = tests;
    }
}

static inline void * __attribute__((alloc_size(1)))
safe_malloc (size_t size)
{
    void *mem = malloc(size);
    if (!mem) {
        fprintf(stderr, " *** ERROR: memory allocation failed\n");
        exit(EXIT_FAILURE);
    }
    return mem;
}

static inline void * __attribute__((alloc_size(2)))
safe_realloc (void *mem, size_t size)
{
    void *new_mem = realloc(mem, size);
    if (!new_mem) {
        fprintf(stderr, " *** ERROR: memory allocation failed\n");
        free(mem);
        exit(EXIT_FAILURE);
    }
    return new_mem;
}

static Suite *
suite_new (const char *name)
{
    static int once = 0;
    if (!once++) {
        atexit(shutdown_tests);
    }

    Suite *suite = safe_malloc(sizeof *tests);
    suite->name = name;
    suite->size = 8;
    suite->num = 0;
    suite->funcs = safe_malloc(sizeof(func_t) * suite->size);
    suite->names = safe_malloc(sizeof(func_t) * suite->size);
    suite->setup = suite->teardown = NULL;
    suite->next = NULL;
    return suite;
}

static inline Suite *
find_suite (const char *name)
{
    Suite *tmp = tests;

    while (tmp != NULL && strcmp(tmp->name, name) != 0) {
        tmp = tmp->next;
    }
    if (tmp == NULL) {
        tmp = suite_new(name);
        tmp->next = tests;
        tests = tmp;
    }

    return tmp;
}

void
ut_register_test (const char *suite, const char * name, func_t f)
{
    Suite *tests = find_suite(suite);

    if (tests->num >= tests->size) {
        tests->size = 2 * tests->size;
        tests->funcs = safe_realloc(tests->funcs, sizeof(func_t) * tests->size);
        tests->names = safe_realloc(tests->names, sizeof(func_t) * tests->size);
    }
    tests->names[tests->num] = name;
    tests->funcs[tests->num] = f;
    tests->num++;
}

void
ut_register_callback (void (*cb)(void), const char *suitename, int type)
{
    Suite *suite = find_suite(suitename);

    switch (type) {
    case 0:
        suite->setup = cb;
        break;
    case 1:
        suite->teardown = cb;
        break;
    }
}

static unsigned int
suite_run (Suite *suite, unsigned int *tests_failed, FILE *logs, unsigned int *assertions_ok, unsigned int *assertions_failed)
{
    for (size_t i = 0; i < suite->num; i++) {

        TestData data = {suite->names[i], 0, 0, logs};

        debug("Running '%s:%s'\n", suite->name, suite->names[i]);
        if (suite->setup) {
            suite->setup();
        }

        suite->funcs[i](&data);

        if (suite->teardown) {
            suite->teardown();
        }
        if (data.assertions_failed > 0) {
            tests_failed++;
            printf(RED "F" NORMAL);
        } else {
            printf(GREEN "." NORMAL);
        }
        *assertions_ok += data.assertions_ok;
        *assertions_failed += data.assertions_failed;
    }
    return suite->num;
}

#define BUFFER_SIZE 256

int
ut_run_all_tests (void)
{
    unsigned int tests_ran = 0;
    unsigned int tests_failed = 0;
    FILE *logs = tmpfile();
    unsigned int assertions_ok = 0;
    unsigned int assertions_failed = 0;

    for (Suite *suite = tests; suite; suite = suite->next) {
        tests_ran += suite_run(suite, &tests_failed, logs, &assertions_ok, &assertions_failed);
    }
    printf("\n\n");
    char buffer[BUFFER_SIZE];
    rewind(logs);
    while (fgets(buffer, BUFFER_SIZE, logs) != NULL) {
        printf("%s", buffer);
    }

    printf("%u assertions succeeded, %u assertions failed\n",
            assertions_ok, assertions_failed);
    printf("%u tests ran\n", tests_ran);
    fclose(logs);
    return tests_failed == 0;
}

void
_ut_assert_func (TestData *data,
                 const char *file,
                 int lineno,
                 int expr,
                 const char *msg, ...)
{
    if (expr) {
        data->assertions_ok++;
        return;
    }
    data->assertions_failed++;
    fprintf(data->logs, "Assertion in %s%s%s (%s:%d) failed:\n\t",
            BOLD, data->name, NORMAL, file, lineno);
    va_list args;
    va_start(args, msg);
    vfprintf(data->logs, msg, args);
    va_end(args);
    fprintf(data->logs, "\n\n");
}

int __attribute__((weak)) main (void)
{
    return ut_run_all_tests() == 0;
}
