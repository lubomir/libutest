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

struct test_result {
    unsigned int tests_ran;
    unsigned int tests_failed;
    unsigned int tests_crashed;
    unsigned int assertions_ok;
    unsigned int assertions_failed;
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

static void
test_run (Suite *suite, size_t idx, TestData *data)
{
    if (suite->setup) {
        suite->setup();
    }

    suite->funcs[idx](data);

    if (suite->teardown) {
        suite->teardown();
    }
}

static void
suite_run (Suite *suite, struct test_result *results, FILE *logs)
{
    for (size_t i = 0; i < suite->num; i++) {
        debug("Running '%s:%s'\n", suite->name, suite->names[i]);

        TestData data = {suite->names[i], 0, 0, logs};
        int pipe_fd[2];

        if (pipe(pipe_fd) < 0) {
            perror("pipe");
            abort();
        }

        fflush(stdout);
        pid_t pid = fork();
        int status;
        if (pid < 0) {
            perror("fork");
            abort();
        } else if (pid == 0) {
            close(pipe_fd[0]);
            test_run (suite, i, &data);
            write(pipe_fd[1], &data, sizeof data);
            close(pipe_fd[1]);
            exit(0);
        } else {
            close(pipe_fd[1]);
            waitpid(pid, &status, 0);
            read(pipe_fd[0], &data, sizeof data);
            close(pipe_fd[0]);
        }

        if (WIFSIGNALED(status)) {
            results->tests_crashed++;
            printf(RED "C" NORMAL);
        } else if (data.assertions_failed > 0) {
            results->tests_failed++;
            printf(RED "F" NORMAL);
        } else {
            printf(GREEN "." NORMAL);
        }
        results->assertions_ok += data.assertions_ok;
        results->assertions_failed += data.assertions_failed;
    }
    results->tests_ran += suite->num;
}

#define BUFFER_SIZE 256

int
ut_run_all_tests (void)
{
    struct test_result results = { 0, 0, 0, 0, 0 };
    FILE *logs = tmpfile();

    for (Suite *suite = tests; suite; suite = suite->next) {
        suite_run(suite, &results, logs);
    }
    printf("\n\n");
    char buffer[BUFFER_SIZE];
    rewind(logs);
    while (fgets(buffer, BUFFER_SIZE, logs) != NULL) {
        printf("%s", buffer);
    }

    printf("%u assertions succeeded, %u assertions failed\n",
            results.assertions_ok, results.assertions_failed);
    printf("%u tests ran, %u failed, %u crashed\n",
            results.tests_ran, results.tests_failed, results.tests_crashed);
    fclose(logs);
    return results.tests_failed == 0;
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
