#include <config.h>

#include "timer.h"
#include "utest.h"
#include "utils.h"

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

typedef struct suite {
    const char *name;
    size_t size;
    size_t num;
    struct {
        const char *name;
        UtFunc func;
    } *tests;
    UtCallback setup;
    UtCallback teardown;
    struct suite *next;
} Suite;

struct ut_test_data {
    const char *name;
    unsigned int assertions_ok;
    unsigned int assertions_failed;
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
ut_register_test (const char *suite, const char * name, UtFunc f)
{
    Suite *s = find_suite(suite);

    if (s->num >= s->size) {
        s->size *= 2;
        s->tests = safe_realloc(s->tests, sizeof *s->tests * s->size);
    }
    s->tests[s->num].name = name;
    s->tests[s->num].func = f;
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

static void
test_run (Suite *suite, size_t idx, UtTestData *data)
{
    if (suite->setup) {
        suite->setup();
    }

    suite->tests[idx].func(data);

    if (suite->teardown) {
        suite->teardown();
    }
}

static void
suite_run (Suite *suite, struct test_result *results, FILE *logs)
{
    for (size_t i = 0; i < suite->num; ++i) {
        debug("Running '%s:%s'\n", suite->name, suite->tests[i].name);

        UtTestData data = {suite->tests[i].name, 0, 0, logs};
        int pipe_fd[2];

        if (pipe(pipe_fd) < 0) {
            perror("pipe");
            abort();
        }

        fflush(stdout);
        timer_start(results->timer);
        pid_t pid = fork();
        int status;
        if (pid < 0) {
            perror("fork");
            abort();
        } else if (pid == 0) {
            close(pipe_fd[0]);
            test_run(suite, i, &data);
            write(pipe_fd[1], &data, sizeof data);
            close(pipe_fd[1]);
            fclose(logs);
            exit(EXIT_SUCCESS);
        } else {
            close(pipe_fd[1]);
            waitpid(pid, &status, 0);
            timer_stop(results->timer);
            read(pipe_fd[0], &data, sizeof data);
            close(pipe_fd[0]);
        }

        if (WIFSIGNALED(status)) {
            ++results->tests_crashed;
            fprintf(logs, "Crash in %s%s%s\n\tKilled with signal %s%d%s (%s)\n\n",
                    BOLD, data.name, NORMAL,
                    BOLD, WTERMSIG(status), NORMAL,
                    strsignal(WTERMSIG(status)));
            putc_color('C', RED);
        } else if (data.assertions_failed > 0) {
            ++results->tests_failed;
            putc_color('F', RED);
        } else {
            putc_color('.', GREEN);
        }
        results->assertions_ok += data.assertions_ok;
        results->assertions_failed += data.assertions_failed;
    }
    results->tests_ran += suite->num;
}

int
ut_run_all_tests (void)
{
    struct test_result results = { NULL, 0, 0, 0, 0, 0 };
    FILE *logs = tmpfile();
    results.timer = timer_new();

    for (Suite *suite = global_tests; suite; suite = suite->next) {
        suite_run(suite, &results, logs);
    }
    printf("\n\n");

    rewind(logs);
    copy_from_to(logs, stdout);

    printf("%u assertions succeeded, %u assertions failed\n",
            results.assertions_ok, results.assertions_failed);
    printf("%u tests ran, %u failed, %u crashed\n",
            results.tests_ran, results.tests_failed, results.tests_crashed);
    printf("Time elapsed: %.3f sec\n", timer_get_elapsed(results.timer));
    fclose(logs);
    timer_free(results.timer);
    return results.tests_failed == 0;
}

void
_ut_assert_func (UtTestData *data,
                 const char *file,
                 int lineno,
                 int expr,
                 const char *msg, ...)
{
    if (expr) {
        ++data->assertions_ok;
        return;
    }
    ++data->assertions_failed;
    fprintf(data->logs, "Assertion in %s%s%s (%s:%d) failed:\n\t",
            BOLD, data->name, NORMAL, file, lineno);
    va_list args;
    va_start(args, msg);
    vfprintf(data->logs, msg, args);
    va_end(args);
    fprintf(data->logs, "\n\n");
}

void
_ut_assert_equal_string(UtTestData *data, const char *file, int line,
                        const char *expected, const char *actual)
{
    if (expected == NULL && actual == NULL) {
        ++data->assertions_ok;
        return;
    }
    if (expected && actual && strcmp(expected, actual) == 0) {
        ++data->assertions_ok;
        return;
    }
    ++data->assertions_failed;
    fprintf(data->logs, "Assertion in %s%s%s (%s:%d) failed:\n\t",
            BOLD, data->name, NORMAL, file, line);

    if (expected == NULL) {
        fprintf(data->logs, "Expected NULL, got <%s%s%s>\n\n",
                BOLD, actual, NORMAL);
        return;
    }
    if (actual == NULL) {
        fprintf(data->logs, "Expected <%s%s%s>, got NULL\n\n",
                BOLD, expected, NORMAL);
        return;
    }

    fprintf(data->logs, "Expected <%s%s%s>\n", BOLD, expected, NORMAL);
    fprintf(data->logs, "\tGot      <%s", BOLD);
    size_t len = strlen(expected);
    for (size_t i = 0; actual[i] != 0; ++i) {
        if (i < len && actual[i] != expected[i]) {
            fprintf(data->logs, "%s%c%s%s", RED, actual[i], NORMAL, BOLD);
        } else {
            fputc(actual[i], data->logs);
        }
    }
    fprintf(data->logs, "%s>\n\n", NORMAL);
}
