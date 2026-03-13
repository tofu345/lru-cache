#ifndef HASH_TABLE_H
#define HASH_TABLE_H

// https://benhoyt.com/writings/hash-table-in-c/

#include <stddef.h>

typedef struct {
    const char *key; // key is NULL if this slot is empty
    void *value;
} hash_table_entry;

typedef struct {
    hash_table_entry *entries; // hash slots
    size_t length, // size of entries array
           capacity; // number of items in hash table
} hash_table;

void ht_init(hash_table *);

void ht_free(hash_table *);

void *ht_get(hash_table *, const char *key);

const char *ht_set(hash_table *, const char *key, void *value);

void ht_remove(hash_table *table, const char *key);

typedef struct {
    // i know you won't listen but don't change these fields directly.
    hash_table *_table;
    size_t _index; // current index in `_table->entries`
} hti;

// Return new hash table iterator (for use with ht_next).
hti ht_iterator(hash_table *);

// Return the next item in the hash table or NULL if there are no more items.
const hash_table_entry *ht_next(hti *);

#endif // HASH_TABLE_H
