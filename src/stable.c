#define __S_TABLE_C__
#include "stable.h"
#undef __S_TABLE_C__

#define __S_SRC_FILE__
#include "slist.h"
#undef __S_SRC_FILE__

#include <stdio.h>
#include <stdlib.h>

#define TABLESIZE 500

struct _smap {
    slist** table;
};

long unsigned int
hash(void* val)
{
    long unsigned int    hashvalue;
    const unsigned char* p = (const unsigned char*) val;

    for(hashvalue = 0; *p; val++)
        hashvalue = *p + 31 * hashvalue;

    return hashvalue % TABLESIZE;
}

smap*
smap_new(void)
{
    smap* map = (smap*) malloc(sizeof(smap));

    if(map != NULL)
        map->table = NULL;

    return map;
}

smap*
smap_insert(smap* map, void* key, void* value)
{
    long unsigned int hashvalue = hash((char*) key);

    if(map->table == NULL
       && (map->table = (slist**) calloc(TABLESIZE, sizeof(slist*))) == NULL)
        return NULL;

    slist* current;
    if(map->table[hashvalue] == NULL) {
        if((current = map->table[hashvalue] = slist_new()) == NULL) {
            fprintf(stderr, "out of memory");
            exit(EXIT_FAILURE);
        }
    }

    if(slist_insert_front(current, value) != 0) {
        fprintf(stderr, "out of memory");
        exit(EXIT_FAILURE);
    }

    return map;
}

void
smap_free(smap* map)
{
    if(map != NULL) {
        slist_free(map->table, (FreeFn) smetadata_free);
        free(map);
    }
}
