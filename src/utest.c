#include <config.h>

#include "timer.h"
#include "utest.h"
#include "utils.h"

#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

typedef struct {
    const char *name;
    UtFunc func;
    const char *file;
} Test;

typedef struct suite {
    const char *name;
    size_t size;
    size_t num;
    Test *tests;
    UtCallback setup;
    UtCallback teardown;
    struct suite *next;
} Suite;

struct ut_test_data {
    const char *name;
    const char *file;
    unsigned int assertions_ok;
    unsigned int assertions_failed;
    void **free_array;          /**< Array of pointers to be freed later. */
    UtFreeFunc *free_funcs;
    size_t free_arr_num;
    size_t free_arr_len;
    FILE *logs;
};

struct test_result {
    Timer *timer;
    unsigned int tests_ran;
    unsigned int tests_failed;
    unsigned int tests_crashed;
    unsigned int assertions_ok;
    unsigned int assertions_failed;
};

/**
 * Helper function for printing messages.
 */
static void _ut_vmessage (UtTestData *data, const char *msg, va_list args);


static Suite *global_tests = NULL;

static void __attribute__((destructor))
shutdown_tests (void)
{
    debug("Shutting down tests\n");
    Suite *s = global_tests;
    while (s) {
        global_tests = s->next;
        free(s->tests);
        free(s);
        s = global_tests;
    }
}

static Suite *
suite_new (const char *name)
{
    Suite *suite = safe_malloc(sizeof *suite);
    suite->name = name;
    suite->size = 8;
    suite->tests = safe_malloc(sizeof *suite->tests * suite->size);
    return suite;
}

static inline Suite *
find_suite (const char *name)
{
    Suite *tmp = global_tests;

    while (tmp != NULL && strcmp(tmp->name, name) != 0) {
        tmp = tmp->next;
    }
    if (tmp == NULL) {
        tmp = suite_new(name);
        tmp->next = global_tests;
        global_tests = tmp;
    }

    return tmp;
}

void
ut_register_test (const char *suite, const char * name, UtFunc f,
        const char *file)
{
    Suite *s = find_suite(suite);

    if (s->num >= s->size) {
        s->size *= 2;
        s->tests = safe_realloc(s->tests, sizeof *s->tests * s->size);
    }
    s->tests[s->num].name = name;
    s->tests[s->num].func = f;
    s->tests[s->num].file = file;
    ++s->num;
}

void
ut_register_callback (UtCallback cb, const char *suitename, UtCallbackType type)
{
    Suite *suite = find_suite(suitename);

    switch (type) {
    case UT_CALLBACK_SETUP:
        suite->setup = cb;
        break;
    case UT_CALLBACK_TEARDOWN:
        suite->teardown = cb;
        break;
    }
}

/**
 * Run all free functions and reset data structure related to them.
 */
static void
run_free_funcs(UtTestData *data)
{
    for (size_t i = 0; i < data->free_arr_num; ++i) {
        data->free_funcs[i](data->free_array[i]);
    }
    free(data->free_array);
    free(data->free_funcs);
    data->free_array = NULL;
    data->free_funcs = NULL;
    data->free_arr_num = data->free_arr_len = 0;
}

static void
test_run (Suite *suite, Test *test, UtTestData *data)
{
    if (suite->setup) {
        suite->setup();
    }

    test->func(data);

    if (suite->teardown) {
        suite->teardown();
    }

    run_free_funcs(data);
}

static bool
test_run_forked (Suite *suite, Test *test, Timer *timer,
        UtTestData *data, int *status, FILE *logs)
{
    int pipe_fd[2];

    if (pipe(pipe_fd) < 0) {
        perror("pipe");
        abort();
    }

    fflush(stdout);
    fflush(logs);
    timer_start(timer);
    pid_t pid = fork();
    ssize_t len;
    if (pid < 0) {
        perror("fork");
        abort();
    } else if (pid == 0) {
        close(pipe_fd[0]);
        test_run(suite, test, data);
        safe_write(pipe_fd[1], data, sizeof *data);
        close(pipe_fd[1]);
        fclose(logs);
        timer_free(timer);
        exit(EXIT_SUCCESS);
    } else {
        close(pipe_fd[1]);
        waitpid(pid, status, 0);
        timer_stop(timer);
        len = read(pipe_fd[0], data, sizeof *data);
        close(pipe_fd[0]);
    }

    if (len != sizeof *data && !WIFSIGNALED(*status)) {
        error("failed to read data");
        return false;
    }

    return true;
}

static void
suite_run (Suite *suite, struct test_result *results,
           FILE *logs, UtFlags flags)
{
    for (size_t i = 0; i < suite->num; ++i) {
        debug("Running '%s:%s'\n", suite->name, suite->tests[i].name);

        UtTestData data = {suite->tests[i].name, suite->tests[i].file, 0, 0, NULL, NULL, 0, 0, logs};
        int status;
        if (!(flags & UT_NO_FORK)) {
            if (!test_run_forked(suite, &suite->tests[i], results->timer, &data,
                        &status, logs))
                continue;
        } else {
            timer_start(results->timer);
            test_run(suite, &suite->tests[i], &data);
            timer_stop(results->timer);
        }

        if (!(flags & UT_NO_FORK) && WIFSIGNALED(status)) {
            ++results->tests_crashed;
            fprintf(logs, "Crash in "_ut_INBOLD("%s")" (%s)\n\t"
                    "Killed with signal "_ut_INBOLD("%d")" (%s)\n\n",
                    data.name, suite->tests[i].file,
                    WTERMSIG(status), strsignal(WTERMSIG(status)));
            putc_color('C', RED, flags);
        } else if (data.assertions_failed > 0) {
            ++results->tests_failed;
            putc_color('F', RED, flags);
        } else {
            putc_color('.', GREEN, flags);
        }
        results->assertions_ok += data.assertions_ok;
        results->assertions_failed += data.assertions_failed;
    }
    results->tests_ran += suite->num;
}

int
ut_run_all_tests (UtFlags flags)
{
    struct test_result results = { NULL, 0, 0, 0, 0, 0 };
    FILE *logs = tmpfile();
    results.timer = timer_new();

    for (Suite *suite = global_tests; suite; suite = suite->next) {
        suite_run(suite, &results, logs, flags);
    }

    if (!(flags & UT_QUIET)) {
        fputs("\n\n", stdout);

        rewind(logs);
        copy_from_to(logs, stdout, flags);

        printf("%u assertions succeeded, %u assertions failed\n",
                results.assertions_ok, results.assertions_failed);
        printf("%u tests ran, %u failed, %u crashed\n",
                results.tests_ran, results.tests_failed, results.tests_crashed);
        printf("Time elapsed: %.3f sec\n", timer_get_elapsed(results.timer));
    }
    fclose(logs);
    timer_free(results.timer);
    return results.tests_failed + results.tests_crashed == 0;
}

void
_ut_assert_func (UtTestData *data, int lineno, int expr, const char *msg, ...)
{
    if (expr) {
        _ut_pass(data);
        return;
    }
    _ut_fail(data, lineno);

    va_list args;
    va_start(args, msg);
    _ut_vmessage(data, msg, args);
    va_end(args);
}

void
_ut_assert_equal_string (UtTestData *data, int line,
                         const char *expected, const char *actual)
{
    if (expected == NULL && actual == NULL) {
        _ut_pass(data);
        return;
    }
    if (expected && actual && strcmp(expected, actual) == 0) {
        _ut_pass(data);
        return;
    }
    _ut_fail(data, line);

    if (expected == NULL) {
        _ut_message(data, "Expected NULL, got <"_ut_INBOLD("%s")">", actual);
        return;
    }
    if (actual == NULL) {
        _ut_message(data, "Expected <"_ut_INBOLD("%s")">, got NULL", expected);
        return;
    }

    fprintf(data->logs, "\tExpected <"_ut_INBOLD("%s")">\n", expected);
    fputs("\tGot      <" BOLD, data->logs);
    size_t len = strlen(expected);
    for (size_t i = 0; actual[i] != 0; ++i) {
        if (i < len && actual[i] != expected[i]) {
            fprintf(data->logs, RED "%c" NORMAL BOLD, actual[i]);
        } else {
            fputc(actual[i], data->logs);
        }
    }
    fputs(NORMAL ">\n\n", data->logs);
}

void
_ut_fail (UtTestData *data, int line)
{
    ++data->assertions_failed;
    fprintf(data->logs, "Assertion in "_ut_INBOLD("%s")" (%s:%d) failed:\n",
            data->name, data->file, line);
}

void
_ut_message (UtTestData *data, const char *msg, ...)
{
    va_list args;
    va_start(args, msg);
    _ut_vmessage(data, msg, args);
    va_end(args);
}

static void
_ut_vmessage (UtTestData *data, const char *msg, va_list args)
{
    fputc('\t', data->logs);
    vfprintf(data->logs, msg, args);
    fputs("\n\n", data->logs);
}

void
_ut_pass (UtTestData *data)
{
    ++data->assertions_ok;
}

void *
_ut_take_memory(UtTestData *data, void *ptr, UtFreeFunc func)
{
    if (data->free_arr_num >= data->free_arr_len) {
        data->free_arr_len += 8;
        data->free_array = safe_realloc(data->free_array, data->free_arr_len * sizeof (*data->free_array));
        data->free_funcs = safe_realloc(data->free_funcs, data->free_arr_len * sizeof (*data->free_array));
    }
    data->free_array[data->free_arr_num] = ptr;
    data->free_funcs[data->free_arr_num] = func;
    ++data->free_arr_num;
    return ptr;
}
