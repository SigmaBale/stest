#ifndef __S_MDATA_H__
#define __S_MDATA_H__

#if !defined(__S_HEADER__) && !defined(__S_SRC_FILE__)
#error "Internal header, only <stest.h> can be included directly."
#endif

typedef enum {
  ASSERT,
  ASSERT_EQ,
  ASSERT_STR_EQ,
  ASSERT_STR_NEQ,
  ASSERT_NEQ
} assert_t;

typedef struct _smdata smdata;

char *atype_to_str(assert_t atype);

void assert_failed(smdata *metadata);

smdata *smdata_new(assert_t assertion, const char *exp, const char *src,
                   const char *dst, const char *file, const char *line,
                   const char *fn_name);

void smdata_free(smdata *metadata);

void smdata_print(const smdata *metadata);

#endif
