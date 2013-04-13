
Running the tests
=================

Tests written using *libutest* are meant to be compiled into a separate
executable file. Therefore to be able to run it, the needs to be a ``main``
function.

There is a default main provided which you can insert into your program with
``UT_DEFAULT_MAIN``.

.. doxygendefine:: UT_DEFAULT_MAIN

However, if you are not satisfied with it and want something more complex, you
can write your own entrypoint. In that case you will need ``ut_run_all_tests``
function to run the tests.

.. doxygenfunction:: ut_run_all_tests
