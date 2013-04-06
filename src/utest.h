#ifndef UTEST_H
#define UTEST_H

#include <string.h>

#define JOIN(a,b,d) a ## _ ## b ## _ ## d
#define STRINGIFY_(x) #x
#define STRINGIFY(x) STRINGIFY_(x)

#define RED     "\033[1;31m"
#define GREEN   "\033[1;32m"
#define BOLD    "\033[1m"
#define NORMAL  "\033[0m"

#define INBOLD(x) BOLD x NORMAL

#define UT_DIRECT_EQ(x,y) (x == y)
#define UT_STR_EQ(x,y) (strcmp(x,y) == 0)

#define _ut_UT_SUITE_TEST(suite, suitename, tname)                          \
    void JOIN(_ut_test, suite, tname) (void);                               \
    void JOIN(_ut_test_register, suite, tname) (void)                       \
        __attribute__((constructor(__COUNTER__ + 101)));                    \
    void JOIN(_ut_test_register, suite, tname) (void) {                     \
        ut_register_test(suitename,                                         \
                STRINGIFY(tname),                                           \
                JOIN(_ut_test, suite, tname));                              \
    }                                                                       \
    void JOIN(_ut_test, suite, tname) (void)

/**
 */
#define UT_TEST(tname)                                                      \
    _ut_UT_SUITE_TEST(def, __FILE__, tname)

/**
 */
#define UT_SUITE_TEST(suite, tname)                                         \
    _ut_UT_SUITE_TEST(suite, suite, tname)

/**
 */
#define ut_assert(exp) _ut_assert_func(exp, STRINGIFY(exp))

/**
 */
#define ut_assert_null(exp)                                                 \
    _ut_assert_func(exp == NULL, INBOLD(STRINGIFY(exp)) " is not NULL")

/**
 */
#define ut_assert_not_null(exp)                                             \
    _ut_assert_func(exp != NULL, INBOLD(STRINGIFY(exp)) " is NULL")

/**
 */
#define ut_assert_eq_int(expected, actual)                                  \
    ut_assert_eq_with_type(int, UT_DIRECT_EQ, "%d", expected, actual)

/**
 */
#define ut_assert_eq_uint(expected, actual)                                 \
    ut_assert_eq_with_type(unsigned int, UT_DIRECT_EQ, "%u", expected, actual)

/**
 */
#define ut_assert_equal_string(expected, actual)                            \
    ut_assert_eq_with_type(char *, UT_STR_EQ, "%s", expected, actual)


#define ut_assert_eq_with_type(type, eq, fmt, expected, actual)             \
    do {                                                                    \
        type _ut_exp = expected;                                            \
        type _ut_act = actual;                                              \
        _ut_assert_func(eq(_ut_exp, _ut_act),                               \
                "Expected <" INBOLD(fmt) ">, got <" INBOLD(fmt) ">",        \
                _ut_exp, _ut_act);                                          \
    } while (0)


/**
 */
#define UT_SETUP                                                            \
    void _ut_setup (void);                                                  \
    void _ut_setup_register (void) __attribute__((constructor));            \
    void _ut_setup_register (void) {                                        \
        ut_register_callback(_ut_setup, 0);                                 \
    }                                                                       \
    void _ut_setup(void)

/**
 */
#define UT_TEARDOWN                                                         \
    void _ut_teardown (void);                                               \
    void _ut_teardown_register (void) __attribute__((constructor));         \
    void _ut_teardown_register (void) {                                     \
        ut_register_callback(_ut_teardown, 1);                              \
    }                                                                       \
    void _ut_teardown(void)

/**
 */
int ut_run_all_tests(void);

/**
 */
#define UT_DEFAULT_MAIN int main (void) { return ut_run_all_tests() == 0; }

#define _ut_assert_func(exp,...)                                            \
    ut_assert_func(__FILE__, __LINE__, exp, __VA_ARGS__)

void ut_register_test(const char *suite, const char *name, void (*f)(void));
void ut_register_callback(void (*cb)(void), int type);
void ut_assert_func(const char *file, int lineno, int expr, const char *msg, ...);

#endif /* end of include guard: UTEST_H */
