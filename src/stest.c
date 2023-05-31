#include "stest.h"
#include <assert.h>
#include <time.h>
#define __S_TEST_C__
#include "stest_impl.h"
#include "stest_internal.h"
#undef __S_TEST_C__

#include <stdlib.h>

#define PRINT_TEST(filename) \
    fprintf(stdout, "Running test \x1b[1;33m%s\x1b[0m ----> ", filename)

#define PRINT_SUCCESS(time) fprintf(stdout, "\x1b[1;32msucceeded\x1b[0m [%ld]\n", time)

#define PRINT_FAILURE(time) fprintf(stdout, "\x1b[1;31mfailed\x1b[0m [%ld]\n", time)

#define PRINT_METADATA_EXP(metadata)                                      \
    fprintf(stdout,                                                       \
            "\x1b[1;33m%s failed:\x1b[0m %s:%s %s -> EXPRESSION: '%s'\n", \
            metadata->fn_name,                                            \
            metadata->file,                                               \
            metadata->line,                                               \
            atype_to_str(metadata->assertion_type),                       \
            metadata->exp)

#define PRINT_METADATA_NO_EXP(metadata)                                          \
    fprintf(stdout,                                                              \
            "\x1b[1;33m%s failed:\x1b[0m %s:%s %s -> LEFT: '%s', RIGHT: '%s'\n", \
            metadata->fn_name,                                                   \
            metadata->file,                                                      \
            metadata->line,                                                      \
            atype_to_str(metadata->assertion_type),                              \
            metadata->src,                                                       \
            metadata->dst)

Map GlobalTable = { .table = NULL };

Metadata*
metadata_new(assert_t    assertion,
             const char* exp,
             const char* src,
             const char* dst,
             const char* file,
             const char* line,
             const char* fn_name)
{
    Metadata* metadata = (Metadata*) malloc(sizeof(Metadata));
    if(metadata != NULL) {
        metadata->assertion_type = assertion;
        metadata->exp            = exp;
        metadata->src            = src;
        metadata->dst            = dst;
        metadata->file           = file;
        metadata->line           = line;
        metadata->fn_name        = fn_name;
    }
    return metadata;
}

static char*
atype_to_str(assert_t atype)
{
    switch(atype) {
        case ASSERT:
            return "ASSERT";
        case ASSERT_EQ:
            return "ASSERT_EQ";
        case ASSERT_STREQ:
            return "ASSERT_STREQ";
        case ASSERT_NEQ:
            return "ASSERT_NEQ";
    }
}

void
assert_failed(Metadata* metadata)
{
    if(map_insert(&GlobalTable, (void*) metadata->fn_name, metadata) == NULL)
        exit(EXIT_FAILURE);
}
