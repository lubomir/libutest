#include <utest.h>

#include <stdlib.h>

UT_TEST(take_on_success) {
    int *p = ut_take_memory(malloc(10), free);

    ut_assert_not_null(p);
}

UT_TEST(take_on_fail) {
    int *p = ut_take_memory(malloc(10), free);

    ut_assert_null(p);
}
