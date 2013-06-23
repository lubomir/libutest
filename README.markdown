Libutest is very simple unit testing framework for C. It tries to make writing
tests as easy as possible. No more lists of test functions, just put your test
routines into a file and hack away. 

## Instalation

    $ git clone git://github.com/lubomir/libutest.git
    $ cd libutest
    $ autoreconf -i
    $ ./configure --enable-docs
    $ make

If you use Debian or Ubuntu, there is a packaged version at [my PPA].

## Documentation

There is some documentation available on <http://lubomir.github.io/libutest/>.
Runtime options are documented with classic `--help` command-line argument.

[my PPA]: https://launchpad.net/~lubomir-sedlar/+archive/ppa
