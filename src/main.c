#include <config.h>

#include "utest.h"

#include <unistd.h>
#include <getopt.h>
#include <stdio.h>

static struct option options[] = {
    { "help", no_argument, NULL, 'h' },
    { "version", no_argument, NULL, 'V' },
    { NULL, 0, 0, 0 }
};

int __attribute__((weak)) main (int argc, char **argv)
{
    int c;

    while (1) {
        int option_index;

        c = getopt_long(argc, argv, "hV", options, &option_index);
        if (c == -1) {
            break;
        }

        switch (c) {
        case 'h':
            printf("Help!\n");
            break;
        case 'V':
            printf("Version!\n");
            break;
        }
    }

    return ut_run_all_tests() == 0;
}
