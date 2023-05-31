#ifndef __S_TABLE_H__
#define __S_TABLE_H__

#if !defined(__S_HEADER__) && !defined(__S_SRC_FILE__)
#error "Private header, only <stest.h> can be included directly."
#endif

#define __S_HEADER__
#include "slist.h"
#undef __S_HEADER__

#ifndef TABLESIZE
#define TABLESIZE 500
#endif

typedef struct _smap smap;

extern smap __S_GLOBAL_TABLE__;

smap *smap_new(void);

smap *smap_insert(smap *map, void *key, void *value);

void smap_free(smap *map);

slist *smap_key(smap *map, void *key);

#endif
