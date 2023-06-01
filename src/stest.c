#define __S_SRC_FILE__
#include "srunner.h"
#undef __S_SRC_FILE__

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

void
_stest_assert(bool        result,
              const char* expression,
              const char* file,
              const int   line,
              const char* fn_name)
{

    if(!result) {
        smdata* metadata = smdata_new(expression, NULL, NULL, file, line, fn_name);

        if(metadata == NULL) {
            fprintf(stderr, "out of memory");
            exit(EXIT_FAILURE);
        }

        assert_failed(metadata);
    }
}

void
_stest_assert_eq(bool        result,
                 const char* left,
                 const char* right,
                 const char* file,
                 const int   line,
                 const char* fn_name)
{

    if(!result) {
        smdata* metadata = smdata_new(NULL, left, right, file, line, fn_name);

        if(metadata == NULL) {
            fprintf(stderr, "out of memory");
            exit(EXIT_FAILURE);
        }

        assert_failed(metadata);
    }
}
