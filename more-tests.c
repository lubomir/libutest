#include <utest.h>
#include <string.h>

UT_TEST(memset_works) {
    size_t len = 4;
    char buffer[len];
    memset(buffer, 'a', len-1);
    buffer[len-1] = 0;
    ut_assert_equal_string("aaa", buffer);
}
