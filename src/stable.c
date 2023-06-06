#define __S_SRC_FILE__
#include "serror.h"
#include "slist.h"
#include "srunner.h"
#include "stable.h"
#undef __S_SRC_FILE__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
 * Sizes of the Hash Table starting from 101 as a default size.
 * Each size (number of buckets) is a prime number, this is
 * in combination with 'HASHCONST' being also prime, making
 * them coprime. This will aid in minimising collisions.
 */
static const unsigned int PRIMES[] = { 101, 199, 443, 881, 1759, 3517, 7069, 14143, 28307, 56599 };

#define PSIZE sizeof(PRIMES) / sizeof(PRIMES[0])

/*
 * HashTable which stores all the
 * test entries and the metadata associated
 * with them.
 *
 * Each test represents entry in the table
 * aka slist, and each metadata associated
 * with the test is stored as a node in that slist.
 *
 * Singleton pattern, no constructor provided only one
 * static global table __S_GLOBAL_TABLE__.
 */
struct _smap {
    slist**      table;
    unsigned int capacity;
    unsigned int len;
};

/*
 * Global hash table variable that hold all tests and their metadata.
 *
 * Internal table is initialized to NULL at first,
 * because if all the tests are successfull then
 * we won't even touch the Global table for metadata.
 */
smap __S_GLOBAL_TABLE__ = { .table = NULL, .capacity = 0, .len = 0 };

/*
 * Returns the current load factor.
 */
double
_smap_load_factor(void)
{
    return (double) __S_GLOBAL_TABLE__.len / (double) __S_GLOBAL_TABLE__.capacity;
}

/*
 * Hashing function.
 */
long unsigned int
hash(const void* val)
{
    static const unsigned long int HASHCONST = 92821;

    long unsigned int    hashvalue;
    const unsigned char* p = (const unsigned char*) val;

    for(hashvalue = 0; *p != '\0'; p++)
        hashvalue = *p + HASHCONST * hashvalue;

    return hashvalue % __S_GLOBAL_TABLE__.capacity;
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
 * Attempts to expand the smap, if it fails it prints the err msg to
 * stderr and exits with 'EXIT_FAILURE'
 */
void
smap_expand(void)
{
    unsigned int current   = __S_GLOBAL_TABLE__.capacity;
    slist**      new_table = NULL;

    unsigned int prime;
    // Get the new table size
    for(unsigned int i = 0; i < PSIZE; i++) {

        if((prime = PRIMES[i]) > current) {

            if((new_table = _table_new(prime)) == NULL) {
                ERROR_OOM_AND_EXIT;
            }

            __S_GLOBAL_TABLE__.capacity = prime;
        }
    }

    // Exit if the table size would exceed biggest prime
    // There is no way someone would run 55k+ unit tests
    if(new_table == NULL) {
        ERROR_TABLE_MEMORY_EXCEEDED_AND_EXIT;
    }

    // Swap tables
    slist** old_table        = __S_GLOBAL_TABLE__.table;
    __S_GLOBAL_TABLE__.table = new_table;

    // Re-hash all the keys and insert the values
    slist* entry;
    while(current--)
        if((entry = *old_table) != NULL) {
            smdata*     old_val = (smdata*) slist_peek_front(entry);
            const char* old_key = smdata_get_name(old_val);
            smap_insert((void*) old_key, old_val);
        }

    // Free the old table
    free(old_table);
}

/*
 * This function runs when .so (shared library) is getting unloaded.
 * ELF file format contains sections called .ctors and .dtors (constructors,
 * destructors) which contain references to the functions with the attributes
 * constructor for .ctor and destructor for .dtor.
 * .ctors and .dtors require support by loader/linker-script, it is up to linker
 * to organize it and loader to run it (the functions defined in .ctors/.dtors).
 * And so you can imagine on embedded systems (bare metal) this might not be available,
 * meaning the global table might leak memory if its inner table is not freed and was
 * initialized, this is actually not a big problem as the OS should cleanup that memory
 * anyways and we are exiting the program, but it is 'impolite'.
 */
__attribute__((destructor)) void
_smap_cleanup_inner(void)
{
    slist**      table = __S_GLOBAL_TABLE__.table;
    unsigned int size  = __S_GLOBAL_TABLE__.capacity;

    if(table != NULL) {
        while(size--)
            slist_free(*table++, (FreeFn) smdata_free);

        free(table);
    }
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
bool
smap_insert(void* key, void* value)
{
    slist** table = __S_GLOBAL_TABLE__.table;

    if(table == NULL) {

        if((table = (__S_GLOBAL_TABLE__.table = _table_new(PRIMES[0]))) == NULL) {
            ERROR_OOM_AND_EXIT;
        }

        __S_GLOBAL_TABLE__.capacity = PRIMES[0];
    }

    long unsigned int hashvalue = hash(key);

    slist*       current    = __S_GLOBAL_TABLE__.table[hashvalue];
    unsigned int table_size = __S_GLOBAL_TABLE__.capacity;

    for(int i = 1;
        current != NULL && strcmp(smdata_get_name((smdata*) slist_peek_front(current)), key) != 0;
        i++)
    {
        // Quadratic probing
        hashvalue = (hashvalue + (i * i)) % table_size;
        current   = __S_GLOBAL_TABLE__.table[hashvalue];
    }

    bool inserted = false;

    // If list allocation fails exit
    if(current == NULL) {

        if((current = (table[hashvalue] = slist_new())) == NULL) {
            ERROR_OOM_AND_EXIT;
        }

        // Only increment if we have new test entry
        __S_GLOBAL_TABLE__.len++;
        inserted = true;

        // If load factor is exceeded expand the map
        if(_smap_load_factor() >= 0.5)
            smap_expand();
    }

    // Finally insert the value, exit if it fails.
    if(slist_insert_front(current, value) != 0) {
        ERROR_OOM_AND_EXIT;
    }

    return inserted;
}

void*
smap_key(void* key)
{
    slist** table = __S_GLOBAL_TABLE__.table;

    if(table != NULL && key != NULL)
        return table[hash(key)];

    return NULL;
}
