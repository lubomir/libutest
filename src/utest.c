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

typedef void (*func_t)(void);
typedef struct suite Suite;

struct suite {
    const char *name;
    size_t size;
    size_t num;
    const char const **names;
    func_t *funcs;
    func_t setup;
    func_t teardown;
    struct suite *next;
};

static Suite *tests = NULL;
static const char *current_test_name = NULL;
static unsigned int assertions_ok = 0;
static unsigned int assertions_failed = 0;
static FILE * logs = NULL;

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

static inline
Suite * find_suite (const char *name)
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
ut_register_test (const char *suite, const char const *name, func_t f)
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
suite_run (Suite *suite, unsigned int *tests_failed)
{
    for (size_t i = 0; i < suite->num; i++) {
        unsigned int old_fails = assertions_failed;
        debug("Running '%s:%s'\n", suite->name, suite->names[i]);
        if (suite->setup) {
            suite->setup();
        }
        current_test_name = suite->names[i];
        suite->funcs[i]();
        if (suite->teardown) {
            suite->teardown();
        }
        if (old_fails != assertions_failed) {
            tests_failed++;
            printf(RED "F" NORMAL);
        } else {
            printf(GREEN "." NORMAL);
        }
    }
    return suite->num;
}

#define BUFFER_SIZE 256

int
ut_run_all_tests (void)
{
    unsigned int tests_ran = 0;
    unsigned int tests_failed = 0;
    logs = tmpfile();

    for (Suite *suite = tests; suite; suite = suite->next) {
        tests_ran += suite_run(suite, &tests_failed);
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
ut_assert_func (const char *file,
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
