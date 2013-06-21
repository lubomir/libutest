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
complex project with sophisticated build system, you may be better of with
something more established, like [cutter].

## Dependencies

To compile from Git, you need autotools and a C compiler that understands GCC
`__attribute__` syntax and a bit of C99. Obviously, GCC works fine, but Clang
can also be used. It should probably work on any platform that runs a
satisfactory compiler, but only GNU/Linux was tested.

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

## Documentation

 * [Defining tests](pages/defining-tests.html)
 * [Writing tests](pages/writing-tests.html)
 * [Running tests](pages/running-tests.html)
 * [Compiling tests](pages/compiling-tests.html)


[libutest repository]: https://github.com/lubomir/libutest/issues
[cutter]: http://cutter.sourceforge.net/
[hakyll]: http://jaspervdj.be/hakyll/
