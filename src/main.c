#include <config.h>

#include "utest.h"

int __attribute__((weak)) main (void)
{
    return ut_run_all_tests() == 0;
}
