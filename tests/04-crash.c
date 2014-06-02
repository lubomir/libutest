#include <utest.h>
#include <stdio.h>

UT_TEST(crash_one) {
    int *p = NULL;
    *p = 1;
}

UT_TEST(crash_two) {
    int *p = NULL;
    int x = *p;
    printf("%d\n", x);
}
