/**
 * @file utest.h    Public definitions for libutest
 */
#ifndef UTEST_H
#define UTEST_H

#include <string.h>

/** @cond HIDDEN */
#define STRINGIFY_(x) #x
#define STRINGIFY(x) STRINGIFY_(x)

#define RED     "\033[1;31m"
#define GREEN   "\033[1;32m"
#define BOLD    "\033[1m"
#define NORMAL  "\033[0m"

#define INBOLD(x) BOLD x NORMAL

/**
 * Direct comparison of two values.
 */
#define UT_DIRECT_EQ(x,y) (x == y)
/**
 * Comparison of strings using strcmp().
 */
#define UT_STR_EQ(x,y) (strcmp(x,y) == 0)

/**
 * Internal macro for defining tests.
 */
#define _ut_UT_SUITE_TEST(suite, suitename, tname)                          \
    static void _ut_test_##suite##_##tname (void);                          \
    static void _ut_test_register_##suite##_##tname (void)                  \
        __attribute__((constructor(__COUNTER__ + 101)));                    \
    static void _ut_test_register_##suite##_##tname (void) {                \
        ut_register_test(suitename,                                         \
                STRINGIFY(tname),                                           \
                _ut_test_##suite##_##tname);                                \
    }                                                                       \
    static void _ut_test_##suite##_##tname (void)

/**
 * Internal macro for defining setup function.
 */
#define _ut_UT_SETUP(id, suitename)                                         \
    void _ut_setup_##id (void);                                             \
    void _ut_setup_##id##_register (void) __attribute__((constructor));     \
    void _ut_setup_##id##_register (void) {                                 \
        ut_register_callback(_ut_setup_##id, suitename, 0);                 \
    }                                                                       \
    void _ut_setup_##id(void)

/**
 * Internal helper macro for writing simple assertions.
 */
#define ut_assert_eq_with_type(type, eq, fmt, expected, actual)             \
    do {                                                                    \
        type _ut_exp = expected;                                            \
        type _ut_act = actual;                                              \
        ut_assert_func(eq(_ut_exp, _ut_act),                               \
                "Expected <" INBOLD(fmt) ">, got <" INBOLD(fmt) ">",        \
                _ut_exp, _ut_act);                                          \
    } while (0)

/**
 * Internal helper macro for writing teardown functions.
 */
#define _ut_UT_TEARDOWN(id, suitename)                                      \
    void _ut_teardown_##id (void);                                          \
    void _ut_teardown_##id##_register (void) __attribute__((constructor));  \
    void _ut_teardown_##id##_register (void) {                              \
        ut_register_callback(_ut_teardown_##id, suitename, 1);              \
    }                                                                       \
    void _ut_teardown_##id(void)

/**
 * Internal function to register a test.
 *
 * @param suite name of the test suite
 * @param name  name of the test
 * @param f     function to run
 */
void ut_register_test(const char *suite, const char *name, void (*f)(void));

/**
 * Internal function to register setup/teardown functions.
 *
 * @param cb    function to be called
 * @param suite name of the modified test suite
 * @param type  0 for setup, 1 for teardown
 */
void ut_register_callback(void (*cb)(void), const char *suite, int type);

/** @endcond */

/**
 * Define a new test.
 *
 * @param tname name of the new test
 */
#define UT_TEST(tname)                                                      \
    _ut_UT_SUITE_TEST(def, __FILE__, tname)

/**
 * Define new test in a suite.
 *
 * @param suite name of the suite
 * @param tname test name
 */
#define UT_SUITE_TEST(suite, tname)                                         \
    _ut_UT_SUITE_TEST(suite, STRINGIFY(suite), tname)

/**
 * Fail the test unless exp evaluates to true.
 */
#define ut_assert(exp) ut_assert_func(exp, STRINGIFY(exp))

/**
 */
#define ut_assert_null(exp)                                                 \
    ut_assert_func(exp == NULL, INBOLD(STRINGIFY(exp)) " is not NULL")

/**
 */
#define ut_assert_not_null(exp)                                             \
    ut_assert_func(exp != NULL, INBOLD(STRINGIFY(exp)) " is NULL")

/**
 */
#define ut_assert_equal_int(expected, actual)                                  \
    ut_assert_eq_with_type(int, UT_DIRECT_EQ, "%d", expected, actual)

/**
 */
#define ut_assert_equal_uint(expected, actual)                                 \
    ut_assert_eq_with_type(unsigned int, UT_DIRECT_EQ, "%u", expected, actual)

/**
 */
#define ut_assert_equal_string(expected, actual)                            \
    ut_assert_eq_with_type(char *, UT_STR_EQ, "%s", expected, actual)


/**
 */
#define UT_SUITE_SETUP(suitename)                                           \
    _ut_UT_SETUP(suitename, STRINGIFY(suitename))

/**
 */
#define UT_SETUP                                                            \
    _ut_UT_SETUP(, __FILE__)

/**
 */
#define UT_SUITE_TEARDOWN(suitename)                                        \
    _ut_UT_TEARDOWN(suitename, STRINGIFY(suitename))

/**
 */
#define UT_TEARDOWN                                                         \
    _ut_UT_TEARDOWN(, __FILE__)


/**
 * Run all defined tests.
 *
 * @return  number of failed tests
 */
int ut_run_all_tests(void);

/**
 * This macro expands to a simple main. This main function runs all the
 * defined tests and then exits with number of failed tests.
 */
#define UT_DEFAULT_MAIN int main (void) { return ut_run_all_tests() == 0; }

/**
 * Frontend for _ut_assert_func(). This macro automatically inserts correct
 * file name and line number.
 */
#define ut_assert_func(exp,...)                                            \
    _ut_assert_func(__FILE__, __LINE__, exp, __VA_ARGS__)

/**
 * Function implementing the assertion.
 *
 * This function is intended for writing custom assertions. Direct calls
 * are not advised.
 *
 * @param file  filename where the assertion takes place
 * @param line  line number where the assertion is located
 * @param expr  expression that is tested by the assertion
 * @param msg   printf-like format string to print error message in case
 *              assertion fails
 */
void _ut_assert_func(const char *file, int line, int expr, const char *msg, ...);

#endif /* end of include guard: UTEST_H */
