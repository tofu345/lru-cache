#ifndef LRU_CACHE_H
#define LRU_CACHE_H

#include "dll.h"
#include "ht.h"

typedef struct {
    const char *key;
    void *value;
} lru_data;

typedef struct {
    doubly_linked_list dll;
    hash_table ht;
    size_t capacity;
} lru_cache;

void lru_init(lru_cache *, size_t initial_capacity);
void lru_free(lru_cache *);

void *lru_get(lru_cache *, const char *key);

void lru_put(lru_cache *, const char *key, void *value);

#endif
