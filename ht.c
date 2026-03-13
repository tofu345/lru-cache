#include "ht.h"

#include <assert.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#define INITIAL_CAPACITY 16

hash_table *ht_new(void)
{
    hash_table *table = malloc(sizeof(hash_table));
    if (table == NULL) return NULL;

    table->length = 0;
    table->capacity = INITIAL_CAPACITY;
    table->entries = calloc(INITIAL_CAPACITY, sizeof(hash_table_entry));
    if (table->entries == NULL)
    {
        free(table);
        return NULL;
    }

    return table;
}

void ht_free(hash_table *table)
{
    for (size_t i = 0; i < table->capacity; i++)
    {
        free((void *)table->entries[i].key);
    }
    free(table->entries);
    free(table);
}

static uint64_t hash_key(const char *key)
{
    uint64_t hash = 14695981039346656037UL;
    for (const char *p = key; *p != '\0'; p++)
    {
        hash ^= (uint64_t)(unsigned char)(*p);
        hash *= 1099511628211UL;
    }
    return hash;
}

// get `hash_table_entry` where key is or could be located,
// stores hash of [key] in [keyHash] if not NULL.
hash_table_entry *
get_entry(hash_table_entry *entries, size_t capacity, const char *key,
          uint64_t *keyHash)
{
    uint64_t hash = hash_key(key);
    if (keyHash != NULL) *keyHash = hash;

    size_t index = (size_t)(hash & (uint64_t)(capacity - 1));

    // loop until an empty entry
    while (entries[index].key != NULL)
    {
        if (strcmp(key, entries[index].key) == 0)
            return &entries[index];

        // move to next slot (linear probing)
        index++;

        // wrap around
        if (index >= capacity)
            index = 0;
    }

    return &entries[index];
}

void *ht_get(hash_table *table, const char *key)
{
    assert(key != NULL);
    hash_table_entry *entry =
        get_entry(table->entries, table->capacity, key, NULL);
    if (entry->key == NULL) return NULL;
    return entry->value;
}

static const char *
set_entry(hash_table_entry *entries, size_t *length, size_t capacity,
          const char *key, void *value)
{
    hash_table_entry *entry = get_entry(entries, capacity, key, NULL);
    if (entry->key != NULL)
    {
        entry->value = value;
        return entry->key;
    }

    if (length != NULL)
    {
        key = strdup(key);
        if (key == NULL) return NULL;
        (*length)++;
    }

    entry->key = (char *)key;
    entry->value = value;
    return key;
}

static bool ht_expand(hash_table *table)
{
    size_t new_capacity = table->capacity * 2;
    if (new_capacity < table->capacity)
        return false; // overflow (capacity too big)

    hash_table_entry *new_entries = calloc(new_capacity, sizeof(hash_table_entry));
    if (new_entries == NULL) return NULL;

    for (size_t i = 0; i < table->capacity; i++)
    {
        hash_table_entry *entry = &table->entries[i];
        if (entry->key != NULL)
        {
            set_entry(new_entries, NULL, new_capacity, entry->key, entry->value);
        }
    }

    free(table->entries);
    table->entries = new_entries;
    table->capacity = new_capacity;
    return true;
}

const char *ht_set(hash_table *table, const char *key, void *value)
{
    assert(key != NULL);
    if (value == NULL) return NULL;

    if (table->length >= table->capacity / 2)
        if (!ht_expand(table))
            return NULL;

    return set_entry(table->entries, &table->length, table->capacity,
                     key, value);
}

void ht_remove(hash_table *table, const char *key)
{
    assert(key != NULL);

    uint64_t hash;
    hash_table_entry *entry =
        get_entry(table->entries, table->capacity, key, &hash);
    if (entry->key == NULL) return;

    // find last collision
    size_t index = entry - table->entries;
    size_t last = index + 1;
    if (last >= table->capacity) last = 0;

    while (table->entries[last].key != NULL)
    {
        if (hash_key(table->entries[last].key) != hash)
        {
            // the previous entry was the last collision
            last--;
            break;
        }

        last++;
        if (last >= table->capacity)
            last = 0;
    }

    // set [key] entry to last collision
    free((void *)entry->key);
    table->entries[index] = table->entries[last];
    table->entries[last].key = NULL;
    table->length--;
}

hti ht_iterator(hash_table *table)
{
    return (hti) {
        ._table = table,
        ._index = 0,
    };
}

const hash_table_entry *ht_next(hti *it)
{
    // loop till we've hit end of entries array.
    hash_table *table = it->_table;
    while (it->_index < table->capacity)
    {
        size_t i = it->_index++;
        if (table->entries[i].key != NULL)
        {
            return &table->entries[i];
        }
    }
    return NULL;
}
