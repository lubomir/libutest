#include <utest.h>

UT_TEST(basic) {
    ut_assert(0);
}

UT_TEST(not_null) {
    int *p = NULL;
    ut_assert_not_null(p);
}

UT_TEST(null) {
    int *p = (int *) 0xCAFE;
    ut_assert_null(p);
}

UT_TEST(equal_int) {
    int i = 1;
    ut_assert_equal_int(2, i);
}

UT_TEST(equal_char) {
    char c = 'a';
    ut_assert_equal_char('c', c);
}

UT_TEST(equal_uint) {
    unsigned u = 2;
    ut_assert_equal_uint(3, u);
}

UT_TEST(equal_string) {
    const char str[] = "bar";
    ut_assert_equal_string("foo", str);
}
