#define __S_SRC_FILE__
#include "smdata.h"
#include "stable.h"
#undef __S_SRC_FILE__

#include <stdio.h>
#include <stdlib.h>

struct _smdata {
    assert_t    assertion_type;
    const char* exp;
    const char* src;
    const char* dst;
    const char* file;
    const char* line;
    const char* fn_name;
};

smdata*
smdata_new(assert_t    assertion,
           const char* exp,
           const char* src,
           const char* dst,
           const char* file,
           const char* line,
           const char* fn_name)
{
    smdata* metadata = (smdata*) malloc(sizeof(smdata));
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

void
smdata_free(smdata* metadata)
{
    if(metadata != NULL)
        free(metadata);
}

void
assert_failed(smdata* metadata)
{
    if(smap_insert(&__S_GLOBAL_TABLE__, (void*) metadata->fn_name, metadata) == NULL) {
        fprintf(stderr, "out of memory\n");
        exit(EXIT_FAILURE);
    }
}

char*
atype_to_str(assert_t atype)
{
    switch(atype) {
        case ASSERT:
            return "ASSERT";
        case ASSERT_EQ:
            return "ASSERT_EQ";
        case ASSERT_STR_EQ:
            return "ASSERT_STR_EQ";
        case ASSERT_STR_NEQ:
            return "ASSERT_STR_NEQ";
        case ASSERT_NEQ:
            return "ASSERT_NEQ";
    }
}

#ifndef NO_COLOR
void
smdata_print(const smdata* metadata)
{
    if(metadata->exp != NULL) {
        fprintf(stdout,
                "\x1b[1;33m%s failed:\x1b[0m %s:%s %s -> EXPRESSION: '%s'\n",
                metadata->fn_name,
                metadata->file,
                metadata->line,
                atype_to_str(metadata->assertion_type),
                metadata->exp);
    } else {
        fprintf(stdout,
                "\x1b[1;33m%s failed:\x1b[0m %s:%s %s -> LEFT: '%s', RIGHT: '%s'\n",
                metadata->fn_name,
                metadata->file,
                metadata->line,
                atype_to_str(metadata->assertion_type),
                metadata->src,
                metadata->dst);
    }
}
#endif

#ifdef NO_COLOR
void
smdata_print(const smdata* metadata)
{
    if(metadata->exp != NULL) {
        fprintf(stdout,
                "%s failed: %s:%s %s -> EXPRESSION: '%s'\n",
                metadata->fn_name,
                metadata->file,
                metadata->line,
                atype_to_str(metadata->assertion_type),
                metadata->exp);
    } else {
        fprintf(stdout,
                "%s failed: %s:%s %s -> LEFT: '%s', RIGHT: '%s'\n",
                metadata->fn_name,
                metadata->file,
                metadata->line,
                atype_to_str(metadata->assertion_type),
                metadata->src,
                metadata->dst);
    }
}
#endif
