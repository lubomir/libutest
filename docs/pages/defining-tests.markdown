---
title: Defining tests
next: writing-tests.html
---

To define a test, use any of the following macros.

Each test belongs to a test suite. The macro `UT_TEST` defines a test in
current file suite. Therefore if for some reason you want to have one test
suite split across more files, you need to explicitly put them into the same
suite using `UT_SUITE_TEST`.

All tests in a single suite will be executed in the order in which they are
defined. The order of the suites themselves is not defined. Order of tests in a
suite split to more files is also not well defined.

`UT_TEST(testname)`

:   This macro creates a new test with given name.

`UT_SUITE_TEST(suite, testname)`

:   Using this macro you can create a test in given test suite. The `suite`
    argument should be name of the test suite. There is no need to create the
    suite beforehand.

### Example

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ {.C}
UT_TEST(firstTest) {
    puts("I will run automatically");
}
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~


## Fixtures

If you have some common code for the tests, you might want to put them into a
separate place. You can use `UT_SETUP` and `UT_TEARDOWN` to define a piece
of code that runs before and after all tests. There are variants of the macro
that assign the code to specific test suite.

There can only be one setup function and one teardown function for each suite.

`UT_SETUP`

:   This macro enables you to register a piece of code that is run before
    each test of registered by `UT_TEST`.

`UT_SUITE_SETUP(suite)`

:   This macro is same as `UT_SETUP`, only the code is run before test of the
    corresponding test suite.

`UT_TEARDOWN`

:   Teardown code is run after each test with no explicit suite.

`UT_SUITE_TEARDOWN(suite)`

:   This code is run after each test in given suite.

### Example

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ {.C}
UT_SETUP {
    puts("Setup 1");
}

UT_TEST(testWithSetup) {
    puts("Setup 1 was executed already");
    puts("But teardown will run only after this test");
}

UT_TEARDOWN {
    puts("All done, good job!");
}
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
