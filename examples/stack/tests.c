#include "stack.h"

#include <stdio.h>
#include <utest.h>
#include <stdlib.h>

static Stack *s = NULL;

UT_SETUP {
    s = stack_new();
}

UT_TEARDOWN {
    stack_free(s);
    s = NULL;
}


UT_TEST(new_stack_not_null) {
    ut_assert_not_null(s);
}

UT_TEST(new_stack_is_empty) {
    ut_assert(stack_empty(s));
}

UT_TEST(can_insert) {
    ut_assert(stack_empty(s));
    stack_push(s, 1);
    ut_assert(!stack_empty(s));
}

UT_TEST(can_peek) {
    stack_push(s, 1);
    ut_assert_equal_int(1, stack_peek(s));
    ut_assert_equal_int(1, stack_peek(s));
}

UT_TEST(can_pop) {
    stack_push(s, 1);
    ut_assert(!stack_empty(s));
    ut_assert_equal_int(1, stack_pop(s));
    ut_assert(stack_empty(s));
}

UT_TEST(stack_size) {
    ut_assert_equal_uint(0, stack_size(s));
    stack_push(s, 1);
    ut_assert_equal_uint(1, stack_size(s));
    stack_push(s, 1);
    ut_assert_equal_uint(2, stack_size(s));
}

UT_TEST(push_many) {
    for (int i = 0; i < 1000; ++i) {
        stack_push(s, i);
    }
    ut_assert_equal_uint(1000, stack_size(s));
}

UT_TEST(retrieve_in_reverse_order) {
    stack_push(s, 1);
    stack_push(s, 2);
    stack_push(s, 3);
    ut_assert_equal_int(3, stack_pop(s));
    ut_assert_equal_int(2, stack_pop(s));
    ut_assert_equal_int(1, stack_pop(s));
    ut_assert(stack_empty(s));
}

UT_TEST(contains_exactly_pushed_items) {
    for (int i = 1; i <= 1000; ++i) {
        stack_push(s, i);
    }
    int sum = 0;
    unsigned num = 0;
    while (stack_size(s) > 0) {
        num++;
        sum += stack_pop(s);
    }
    ut_assert(stack_empty(s));
    ut_assert_equal_uint(1000, num);
    ut_assert_equal_int(500500, sum);
}
