---
title: Documentation of libutest
---

`libutest` is a very simple unit testing framework for C. Its main advantage is
that it does not force you to write lists of all test function. Instead, it
finds them automatically. Magic!

File found issues at GitHub in [libutest repository].

## Who it is for

This library/framework is supposed to be used with simple programs. Initially,
it was designed to simplify testing programs written as assignment for
introductory courses, where simplicity is essential.

Testing with `libutest` consists of writing the tests into a file, compiling it
into an executable and you are ready to go. However, if you are working on a
complex project with sophisticated build system, you may be better off with
something more established, like [cutter].

## Dependencies

To compile from Git, you need autotools and a C compiler that understands GCC
`__attribute__` syntax and a bit of C99. Obviously, GCC works fine, but Clang
can also be used. It should probably work on any platform that runs a
satisfactory compiler, but only GNU/Linux was tested.

On Windows, version 0.4 was tested on [MinGW] and does not work. Compiling from
tarball under [Cygwin] is possible (given you include `main()` function and
ignore compiler warnings).

This website is built from Markdown files using [Hakyll], which is quite
dependency heavy. However, for actually using the library this is very much
useless.

## Installation

So far, you need to install `libutest` by building from source yourself.
It uses the classic autotools build. First obtain the contents of the
repository, then run `configure` and `make`.

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ {.bash}
$ git clone git://github.com/lubomir/libutest.git
$ autoreconf -i
$ ./configure
$ make
$ make install
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

### Packages

There are prepared Debian/Ubuntu packages at [my PPA].

## Documentation

The only public interface of the library is the header file `utest.h`.

 * [Defining tests](pages/defining-tests.html)
 * [Writing tests](pages/writing-tests.html)
 * [Running tests](pages/running-tests.html)
 * [Compiling tests](pages/compiling-tests.html)

### Quick overview

The `libutest` library makes it very convenient to write unit tests, because it
does not require the programmer to write list of every test routine. This is
accomplished by using GCC `constructor` function attribute, which marks a
function that should be run on program startup.

Each test is actually composed of two functions. One of them is a normal
function with the actual testing code, the other is a constructor one that
registers the test in a global storage. The `main` function can [execute the
tests](pages/running-tests.html) later.

Since writing two functions by hand would not by any better than having to
write some list, preprocessor macros are used (some might say abused) to
actually create the functions. User of this library writes something like:

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ {.c}
UT_TEST(MyTest) {
    /* test code */
}
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

which gets expanded to the following snippet.^[Actually it is a little more
complicated due to the fact that each test belongs into a suite. But that is
the gist of it.]

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ {.c}
static void /* constructor */ _ut_register_MyTest(void) {
    ut_register_test(_ut_test_MyTest, "MyTest");
}
static void _ut_test_MyTest(UtTestData *_ut_data) {
    /* test code */
}
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

The argument to the testing function is hidden from the user and only used by
provided assertions. All functions that access it have a preprocessor macro
wrapper that uses this argument without the user actually having to know.


[libutest repository]: https://github.com/lubomir/libutest/issues
[cutter]: http://cutter.sourceforge.net/
[hakyll]: http://jaspervdj.be/hakyll/
[my PPA]: https://launchpad.net/~lubomir-sedlar/+archive/ppa
[MinGW]: http://mingw.org/
[Cygwin]: http://cygwin.com/
