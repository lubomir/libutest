#include <utest.h>
#include <string.h>
#include <stdlib.h>

UT_TEST(memset_works) {
    const size_t len = 3;
    char buffer[len+1];
    memset(buffer, 'a', len);
    buffer[len] = 0;
    ut_assert_equal_string("aaa", buffer);
}

UT_TEST(strstr_works) {
    char *haystack = "hello world";
    char *needle = "ello";
    ut_assert_not_null(strstr(haystack, needle));
    needle = "hi";
    ut_assert_null(strstr(haystack, needle));
}

UT_TEST(atoi_works) {
    int num;
    num = atoi("5");
    ut_assert_equal_int(5, num);
    num = atoi("15");
    ut_assert_equal_int(15, num);
    num = atoi("-10");
    ut_assert_equal_int(-10, num);
}

UT_DEFAULT_MAIN
