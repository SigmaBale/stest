#define __S_SRC_FILE__
#include "slist.h"
#include "srunner.h"
#include "stable.h"
#undef __S_SRC_FILE__

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
 * Sizes of the Hash Table starting from 101 as a default size.
 * Each size (number of buckets) is a prime number, this is
 * in combination with 'HASHCONST' being also prime, making
 * them coprime. This will aid in minimising collisions.
 */
static const unsigned int PRIMES[]
    = { 101, 199, 443, 881, 1759, 3517, 7069, 14143, 28307, 56599 };

#define PSIZE sizeof(PRIMES) / sizeof(PRIMES[0])

/*
 * HashTable which stores all the
 * test entries and the metadata associated
 * with them.
 *
 * Each test represents entry in the table
 * aka slist, and each metadata associated
 * with the test is stored as a node in that slist.
 */
struct _smap {
    slist**      table;
    unsigned int capacity;
    unsigned int len;
};

/*
 * Global hash table variable that hold all
 * tests and their metadata.
 *
 * Internal table is initialized to NULL at first,
 * because if all the tests are successfull then
 * we won't even touch the Global table for metadata.
 */
smap __S_GLOBAL_TABLE__ = { .table = NULL, .capacity = 0, .len = 0 };

/*
 * Global that sets the default table
 * size when it gets initialized
 * (when the first function fails).
 */

double
_smap_load_factor(smap* map)
{
    return (double) map->len / (double) map->capacity;
}

/*
 * Hashing function.
 */
long unsigned int
hash(const void* val, unsigned int capacity)
{
    static const unsigned long int HASHCONST = 92821;

    long unsigned int    hashvalue;
    const unsigned char* p = (const unsigned char*) val;

    for(hashvalue = 0; *p != '\0'; p++)
        hashvalue = *p + HASHCONST * hashvalue;

    return hashvalue % capacity;
}

/*
 * Just a wrapper function around calloc
 * for cleaner code in other functions.
 */
slist**
_table_new(size_t tablesize)
{
    slist** table = (slist**) calloc(tablesize, sizeof(slist*));
    return table;
}

/*
 * Map constructor.
 * Useless in this library because we
 * are using staticly known global map/table.
 * This is for completion purposes and in case
 * the program gets restructured in a way where
 * global table won't be required.
 */
smap*
smap_new(void)
{
    smap* map = (smap*) malloc(sizeof(smap));

    if(map != NULL)
        map->table = NULL;

    return map;
}

/*
 * Attempts to expand the smap,
 * if it fails (out of memory) it
 * prints the err msg to stderr and
 * exits with 'EXIT_FAILURE'
 */
void
smap_expand(smap* map)
{
    if(map != NULL) {
        unsigned int current   = map->capacity;
        slist**      new_table = NULL;

        unsigned int prime;
        // Get the new table size
        for(unsigned int i = 0; i < PSIZE; i++) {
            if((prime = PRIMES[i]) > current) {
                if((new_table = _table_new(prime)) == NULL) {
                    fprintf(stderr, "out of memory");
                    exit(EXIT_FAILURE);
                }
                map->capacity = prime;
            }
        }

        // Exit if the table size would exceed biggest prime
        // There is no way someone would run 55k+ unit tests
        if(new_table == NULL) {
            fprintf(stderr, "Your table is too big, exiting...");
            exit(EXIT_FAILURE);
        }

        // Swap tables
        slist** old_table = map->table;
        map->table        = new_table;

        // Re-hash all the keys and insert the values
        slist* entry;
        while(--current) {
            if((entry = *old_table) != NULL) {
                smdata*     old_val = (smdata*) slist_peek_front(entry);
                const char* old_key = smdata_get_name(old_val);
                smap_insert(map, (void*) old_key, old_val);
            }
        }

        // Free the old table
        free(old_table);
    }
}

/*
 * Internal only function used in cases
 * where we run out of memory and need
 * to cleanup the inner table before exiting.
 * Used in other modules that rely on the table.
 */
void
_smap_cleanup_inner(smap* map)
{
    if(map != NULL && map->table != NULL)
        for(int size = map->capacity; --size >= 0;)
            slist_free(map->table[size], (FreeFn) smdata_free);
}

/*
 * Inserts the VALUE into the MAP.
 * Key gets hashed and that hash is used for
 * indexing the table, in case of collision
 * open adressing resolves it using quadratic probing.
 *
 * In case process is out of memory the function
 * will print err message to stderr and exit the
 * program with 'EXIT_FAILURE'.
 * **This will leak memory** as there is no way
 * for us to cleanup the user allocated memory.
 */
smap*
smap_insert(smap* map, void* key, void* value)
{
    if(map != NULL) {

        // If load factor is exceeded expand the map
        if(map->len != 0 && _smap_load_factor(map) >= 0.5)
            smap_expand(map);

        slist** table = map->table;

        if(table == NULL) {
            if((table = (map->table = _table_new(PRIMES[0]))) == NULL) {
                fprintf(stderr, "out of memory");
                // Exit if table can't be allocated
                exit(EXIT_FAILURE);
            }
            map->capacity = PRIMES[0];
        }

        long unsigned int hashvalue = hash((char*) key, map->capacity);

        slist*       current    = map->table[hashvalue];
        unsigned int table_size = map->capacity;

        for(int i = 1;
            current != NULL
            && strcmp(smdata_get_name((smdata*) slist_peek_front(current)), key) != 0;
            i++)
        {
            // Quadratic probing
            hashvalue = (hashvalue + (i * i)) % table_size;
            current   = map->table[hashvalue];
        }

        // If list allocation fails exit
        if(current == NULL) {
            if((current = (table[hashvalue] = slist_new())) == NULL) {
                fprintf(stderr, "out of memory");
                _smap_cleanup_inner(map);
                exit(EXIT_FAILURE);
            }
            // Only increment if we have new test entry
            map->len++;
        }

        // Finally insert the value, exit if it fails.
        if(slist_insert_front(current, value) != 0) {
            fprintf(stderr, "out of memory");
            _smap_cleanup_inner(map);
            exit(EXIT_FAILURE);
        }
    }

    return map;
}

void*
smap_key(smap* map, void* key)
{
    if(map != NULL && key != NULL) {
        // Not necessary to exit here, maybe just return NULL,
        // because we are only trying to read from a table that
        // is not even initialized, returning NULL and letting
        // user handle it would be okay but this is test library so lets exit...
        if(map->table == NULL) {
            if((map->table = _table_new(PRIMES[0])) == NULL) {
                fprintf(stderr, "out of memory");
                exit(EXIT_FAILURE);
            }
            map->capacity = PRIMES[0];
        }
        return map->table[hash(key, map->capacity)];
    }

    return NULL;
}

void
smap_free(smap* map)
{
    if(map != NULL) {
        int     size  = map->capacity;
        slist** table = map->table;

        while(size--)
            slist_free(*table++, (FreeFn) smdata_free);

        free(map);
    }
}
