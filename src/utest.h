/**
 * @file utest.h    Public definitions for libutest
 */
#ifndef UTEST_H
#define UTEST_H

#include <stddef.h>

/** @cond HIDDEN */
#define _ut_STRINGIFY_INT(x) #x
#define _ut_STRINGIFY(x) _ut_STRINGIFY_INT(x)

#define _ut_BOLD    "\033[1m"
#define _ut_NORMAL  "\033[0m"

#define _ut_INBOLD(x)   _ut_BOLD x _ut_NORMAL
#define _ut_INBOLD_S(x) _ut_INBOLD(_ut_STRINGIFY(x))

/**
 * Enumeration for callback types: setup and teardown functions. Using these
 * directly is not neecessary.
 */
typedef enum {
    UT_CALLBACK_SETUP,
    UT_CALLBACK_TEARDOWN
} UtCallbackType;

/**
 * Structure with data passed to testing functions.
 * This is an opaque data type, do not access it directly.
 */
typedef struct ut_test_data UtTestData;

/**
 * Type of a test function. Do not use directly.
 */
typedef void (*UtFunc)(UtTestData *);

/**
 * Type of a callback.
 */
typedef void (*UtCallback)(void);

/**
 * Internal macro for defining tests.
 */
#define _ut_UT_SUITE_TEST(suite, suitename, tname)                          \
    static void _ut_test_##suite##_##tname (UtTestData *_ut_test_data);     \
    static void __attribute__((constructor(__COUNTER__ + 101)))             \
    _ut_test_register_##suite##_##tname (void) {                            \
        ut_register_test(suitename, #tname,                                 \
                _ut_test_##suite##_##tname, __FILE__);                      \
    }                                                                       \
    static void _ut_test_##suite##_##tname (UtTestData * _ut_test_data      \
            __attribute__((unused)))

/**
 * Internal macro for defining setup function.
 */
#define _ut_UT_SETUP(id, suitename)                                         \
    static void _ut_setup_##id (void);                                      \
    static void __attribute__((constructor))                                \
    _ut_setup_##id##_register (void) {                                      \
        ut_register_callback(_ut_setup_##id, suitename, UT_CALLBACK_SETUP); \
    }                                                                       \
    static void _ut_setup_##id(void)

/**
 * Internal helper macro for writing simple assertions.
 */
#define _ut_assert_eq_with_type(type, fmt, expected, actual)                \
    do {                                                                    \
        type _ut_exp = expected;                                            \
        type _ut_act = actual;                                              \
        ut_assert_func((_ut_exp) == (_ut_act),                              \
                "Expected <"_ut_INBOLD(fmt)">, got <"_ut_INBOLD(fmt)">",    \
                _ut_exp, _ut_act);                                          \
    } while (0)

/**
 * Internal helper macro for writing teardown functions.
 */
#define _ut_UT_TEARDOWN(id, suitename)                                      \
    static void _ut_teardown_##id (void);                                   \
    static void __attribute__((constructor))                                \
    _ut_teardown_##id##_register (void) {                                   \
        ut_register_callback(_ut_teardown_##id,                             \
                suitename, UT_CALLBACK_TEARDOWN);                           \
    }                                                                       \
    static void _ut_teardown_##id(void)

/**
 * Internal function to register a test.
 *
 * @param suite name of the test suite
 * @param name  name of the test
 * @param f     function to run
 * @param file  file where the test is defined
 */
void ut_register_test(const char *suite, const char *name, UtFunc f,
        const char *file);

/**
 * Internal function to register setup/teardown functions.
 *
 * @param cb    function to be called
 * @param suite name of the modified test suite
 * @param type  0 for setup, 1 for teardown
 */
void ut_register_callback(UtCallback cb, const char *suite, UtCallbackType type);

/** @endcond */

/**
 * Define a new test in global file suite.
 *
 * The test name should be an identifier such as for a function or a variable.
 *
 * @param tname name of the new test
 */
#define UT_TEST(tname)                                                      \
    _ut_UT_SUITE_TEST(def, __FILE__, tname)

/**
 * Define new test in a suite.
 *
 * Both arguments should be identifiers such as for a function or a variable.
 *
 * @param suite name of the suite
 * @param tname test name
 */
#define UT_SUITE_TEST(suite, tname)                                         \
    _ut_UT_SUITE_TEST(suite, #suite, tname)

/**
 * Fail the test unless `exp` evaluates to true.
 *
 * This is the simplest possible assertion. You can use it to perform any kind
 * of test you need. However, using some more specialized assertion, you can
 * get better error message if the assertion fails.
 *
 * @param exp   expression that must evaluate to `TRUE`
 */
#define ut_assert(exp) ut_assert_func(exp, _ut_STRINGIFY(exp))

/**
 * Fail the test unless `exp` evaluates to `NULL`.
 *
 * @param exp   expression that must evaluate to `NULL`
 */
#define ut_assert_null(exp)                                                 \
    ut_assert_func((exp) == NULL, _ut_INBOLD(#exp)" (%p) is not NULL", exp)


/**
 * Fail the test if expression evaluates to `NULL`.
 *
 * @param exp   expression that must evaluate to anything but `NULL`
 */
#define ut_assert_not_null(exp)                                             \
    ut_assert_func((exp) != NULL, _ut_INBOLD_S(exp) " is NULL")

/**
 * Fail the test unless two integer values are the same.
 *
 * @param expected  expected value in the second parameter
 * @param actual    expression that evaluates to integer value
 */
#define ut_assert_equal_int(expected, actual)                               \
    _ut_assert_eq_with_type(int, "%d", expected, actual)

/**
 * Fail the test unless two unsigned integer values are the same.
 *
 * @param expected  expected value in the second parameter
 * @param actual    expression that evaluates to unsigned integer value
 */
#define ut_assert_equal_uint(expected, actual)                              \
    _ut_assert_eq_with_type(unsigned int, "%u", expected, actual)

/**
 * Fail the test unless two strings are both `NULL` or have the same contents.
 *
 * @param expected  expected value of the string
 * @param actual    expression that evaluates to a pointer to `char`
 */
#define ut_assert_equal_string(expected, actual)                            \
    _ut_assert_equal_string(_ut_test_data, __LINE__, expected, actual)

/**
 * Actual string compare assertion.
 *
 * @param data      test data
 * @param line      line from which the assertion was called
 * @param expected  expected string
 * @param actual    actual string
 */
void _ut_assert_equal_string(UtTestData *data, int line,
        const char *expected, const char *actual);

/**
 * Add a setup function to the default file suite.
 *
 * This code will be run before each test in this file that is not explicitly
 * added to a named test suite.
 */
#define UT_SETUP                                                            \
    _ut_UT_SETUP(, __FILE__)

/**
 * Add a setup function to a given test suite.
 *
 * This code will be run before each test in the test suite with given name.
 *
 * @param suitename     the test suite to be extended with this code
 */
#define UT_SUITE_SETUP(suitename)                                           \
    _ut_UT_SETUP(suitename, #suitename)

/**
 * Add a teardown to the default file suite.
 *
 * This code will be run after each test in this file that is not explicitly
 * added to a named test suite.
 */
#define UT_TEARDOWN                                                         \
    _ut_UT_TEARDOWN(, __FILE__)

/**
 * Add a teardown function to a given test suite.
 *
 * This code will be run after each test in the test suite with given name.
 */
#define UT_SUITE_TEARDOWN(suitename)                                        \
    _ut_UT_TEARDOWN(suitename, #suitename)

typedef enum {
    UT_QUIET    = 1 << 0,       /**< Do not print anything at all. */
    UT_NO_FORK  = 1 << 1        /**< Run each test in separate process. */
} UtFlags;

/**
 * Run all defined tests.
 *
 * @param flags     flags affecting the runtime
 * @return          number of failed tests
 */
int ut_run_all_tests(UtFlags flags);

/**
 * Frontend for _ut_assert_func(). This macro automatically inserts correct
 * file name and line number.
 */
#define ut_assert_func(exp,...)                                            \
    _ut_assert_func(_ut_test_data, __LINE__, exp, __VA_ARGS__)

/**
 * Function implementing the assertion.
 *
 * This function is intended for writing custom assertions. Direct calls
 * are not advised.
 *
 * @param data  test data
 * @param line  line number where the assertion is located
 * @param expr  expression that is tested by the assertion
 * @param msg   `printf`-like format string to print error message in case
 *              assertion fails
 */
void _ut_assert_func(UtTestData *data,
                     int line,
                     int expr,
                     const char *msg,
                     ...)
    __attribute__((format(printf, 4, 5)));

/**
 * Wrapper around `_ut_fail`. This prints standard failure message and marks
 * the assertion as failed. The message includes a filename, which is
 * automatically determined from the current test.
 *
 * @param line  line number to be included in the message
 */
#define ut_fail(line)   _ut_fail(_ut_test_data, line)

/**
 * Mark current assertion as failed and print standard failure message.
 *
 * @param data  test data
 * @param line  line number to be included in the message
 */
void _ut_fail(UtTestData *data, int line);

/**
 * Mark current assertion as passed.
 */
#define ut_pass()   _ut_pass(_ut_test_data);

/**
 * Mark current assertion as passed.
 *
 * @param data  test data
 */
void _ut_pass(UtTestData *data);

/**
 * Print detailed message about failed assertion. This macro uses
 * `printf`-like arguments. The message will be properly indented and
 * followed by appropriate number of newlines.
 */
#define ut_message(...) _ut_message(_ut_test_data, __VA_ARGS);

/**
 * Print detailed message about failed assertion.
 *
 * The message will be properly indented and followed by appropriate number of
 * newlines.
 *
 * @param data  test data
 * @param msg   `printf`-like format string
 * @param ...   the rest of arguments
 */
void _ut_message(UtTestData *data, const char *msg, ...)
    __attribute__((format(printf, 2, 3)));

/**
 * Helper function for the generated main() entrypoint. Do not call this
 * function directly. If you want to use provided functionality, use
 * UT_DEFAULT_MAIN instead to generate whole main function.
 */
int _ut_default_main_worker (int argc, char **argv);

/**
 * This macro expands to a simple main. This main function runs all the
 * defined tests and then exits with number of failed tests.
 */
#define UT_DEFAULT_MAIN                                                     \
    int main (int argc, char **argv)                                        \
    { return _ut_default_main_worker(argc, argv); }

#endif /* end of include guard: UTEST_H */
