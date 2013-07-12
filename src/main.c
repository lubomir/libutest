#include <config.h>

#include "utest.h"

#include <unistd.h>
#include <getopt.h>
#include <stdio.h>
#include <string.h>

#define OPTSTRING "hVqf"

static struct option options[] = {
    { "help",       no_argument, NULL, 'h' },
    { "version",    no_argument, NULL, 'V' },
    { "quiet",      no_argument, NULL, 'q' },
    { "no-fork",    no_argument, NULL, 'f' },
    { "color",      required_argument, NULL, 'c' },
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
    puts("  -f, --no-fork   disable forking before running tests");
    puts("  --color=WHEN    customize coloring. WHEN defaults to 'auto' or can");
    puts("                  be 'never' or 'always'");

    printf("\nReport %s bugs to %s\n", PACKAGE_NAME, PACKAGE_BUGREPORT);
    printf("%s homepage: %s\n", PACKAGE_NAME, PACKAGE_URL);
}

static void
version (void)
{
    printf("%s %s\n", PACKAGE_NAME, VERSION);
}

int _ut_default_main_worker (int argc, char **argv)
{
    int c;
    UtFlags flags = 0;

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
            flags |= UT_QUIET;
            break;
        case 'f':
            flags |= UT_NO_FORK;
            break;
        case 'c':
            if (strcmp(optarg, "never") == 0) {
                flags |= UT_COLOR_NEVER;
            } else if (strcmp(optarg, "always") == 0) {
                flags |= UT_COLOR_ALWAYS;
            } else if (strcmp(optarg, "auto") != 0) {
                fprintf(stderr, "Error: unrecognized --color option argument\n");
                return 1;
            }
            break;
        case '?':
            usage(argv[0], stderr);
            return 1;
        }
    }

    return ut_run_all_tests(flags) == 0;
}

int __attribute__((weak))
main (int argc, char **argv)
{
    return _ut_default_main_worker(argc, argv);
}
