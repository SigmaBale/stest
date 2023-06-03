#ifndef __S_TABLE_H__
#define __S_TABLE_H__

#if !defined(__S_HEADER__) && !defined(__S_SRC_FILE__)
#error "Private header, only <stest.h> can be included directly."
#endif

#define __S_SRC_FILE__
#include "slist.h"
#undef __S_SRC_FILE__

typedef struct _smap smap;

extern unsigned long int TABLESIZE;

extern smap __S_GLOBAL_TABLE__;

smap *smap_new(void);

smap *smap_insert(smap *map, void *key, void *value);

void smap_free(smap *map);

void *smap_key(smap *map, void *key);

void _smap_cleanup_inner(smap *map);

#endif
