
Defining tests
==============

To define a test, use any of the following macros.

Each test belongs to a test suite. The macro ``UT_TEST`` defines a test in
current file suite. Therefore if for some reason you want to have one test
suite split across more files, you need to explicitly put them into the same
suite using ``UT_SUITE_TEST``.

All tests in a single suite will be executed in the order in which they are
defined. The order of the suites themselves is not defined. Order of tests in a
suite split to more files is also not well defined.

.. doxygendefine:: UT_TEST
.. doxygendefine:: UT_SUITE_TEST

Fixtures
--------

If you have some common code for the tests, you might want to put them into a
separate place. You can use ``UT_SETUP`` and ``UT_TEARDOWN`` to define a piece
of code that runs before and after all tests. There are variants of the macro
that assign the code to specific test suite.

There can only be one setup function and one teardown function for each suite.

.. doxygendefine:: UT_SETUP
.. doxygendefine:: UT_SUITE_SETUP
.. doxygendefine:: UT_TEARDOWN
.. doxygendefine:: UT_SUITE_TEARDOWN
