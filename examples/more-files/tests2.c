#include <utest.h>

static int global_value = 0;

UT_SETUP {
    global_value = 5;
}

UT_TEARDOWN {
    global_value = 0;
}

UT_TEST(global_is_five) {
    ut_assert_equal_int(5, global_value);
}

UT_TEST(change_of_global) {
    global_value = 3;
    ut_assert_equal_int(3, global_value);
}

UT_TEST(global_is_not_three) {
    ut_assert(3 != global_value);
}
