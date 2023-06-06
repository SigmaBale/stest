#ifndef __S_TABLE_H__
#define __S_TABLE_H__

#if !defined(__S_HEADER__) && !defined(__S_SRC_FILE__)
#error "Private header, only <stest.h> can be included directly."
#endif

#define __S_HEADER__
#include "slist.h"
#undef __S_HEADER__
#include <stdbool.h>

typedef struct _smap smap;

extern unsigned long int TABLESIZE;

extern smap __S_GLOBAL_TABLE__;

bool smap_insert(void *key, void *value);

void *smap_key(void *key);

#endif
