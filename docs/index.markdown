---
title: Documentation of libutest
---

`libutest` is a very simple unit testing framework for C. Its main advantage is
that does not force you to write lists of all test function. Instead, it finds
them automatically. Magic!

File found issues at GitHub in [libutest repository].

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
