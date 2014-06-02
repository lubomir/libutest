#include <utest.h>
#include <stdio.h>

static int global = 0;

UT_SETUP {
    global = 1;
}

UT_TEARDOWN {
    global = 0;
}

UT_TEST(global_is_one) {
    printf("should be %d, is %d\n", 1, global);
}

UT_SUITE_SETUP(suite_two) {
    global = 2;
}

UT_SUITE_TEARDOWN(suite_two) {
    global = 0;
}

UT_SUITE_TEST(suite_two, global_is_two) {
    printf("should be %d, is %d\n", 2, global);
}

UT_SUITE_SETUP(suite_three) {
    global = 3;
}

UT_SUITE_TEARDOWN(suite_three) {
    global = 0;
}

UT_SUITE_TEST(suite_three, global_is_three) {
    printf("should be %d, is %d\n", 3, global);
}
