
Writing tests
=============

In each test, you can use many test assertions.

.. doxygendefine:: ut_assert
.. doxygendefine:: ut_assert_null
.. doxygendefine:: ut_assert_not_null
.. doxygendefine:: ut_assert_equal_int
.. doxygendefine:: ut_assert_equal_uint
.. doxygendefine:: ut_assert_equal_string

Custom assertions
-----------------

You may find yourself in a situation where you need to write your own assertion. There is support for doing that.

.. doxygendefine:: ut_assert_func
.. doxygenfunction:: _ut_assert_func
