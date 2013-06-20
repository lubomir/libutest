#include <config.h>

#include "utest.h"

#include <unistd.h>
#include <getopt.h>
#include <stdio.h>

#define OPTSTRING "hVq"

static struct option options[] = {
    { "help",       no_argument, NULL, 'h' },
    { "version",    no_argument, NULL, 'V' },
    { "quiet",      no_argument, NULL, 'q' },
    { NULL, 0, 0, 0 }
};

static void
usage (const char *exe, FILE *fh)
{
    fprintf(fh, _ut_INBOLD("Usage")": %s [OPTIONS]\n", exe);
}

static void
help (const char *exe)
{
    usage(exe, stdout);

    puts("\n"_ut_INBOLD("Options"));
    puts("  -h, --help      print this help");
    puts("  -V, --version   display version information");
    puts("  -q, --quiet     do not display any output");

    printf("\nReport %s bugs to %s\n", PACKAGE_NAME, PACKAGE_BUGREPORT);
    printf("%s homepage: %s\n", PACKAGE_NAME, PACKAGE_URL);
}

static void
version (void)
{
    printf("%s %s\n", PACKAGE_NAME, VERSION);
}

int __attribute__((weak)) main (int argc, char **argv)
{
    int c;
    UtVerbosityLevel verbosity = UT_NORMAL;

    while (1) {
        c = getopt_long(argc, argv, OPTSTRING, options, NULL);
        if (c == -1) {
            break;
        }

        switch (c) {
        case 'h':
            help(argv[0]);
            return 0;
        case 'V':
            version();
            return 0;
        case 'q':
            verbosity = UT_QUIET;
            break;
        case '?':
            usage(argv[0], stderr);
            return 1;
        }
    }

    return ut_run_all_tests(verbosity) == 0;
}
