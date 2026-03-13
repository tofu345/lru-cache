#include "lru.h"
#include "dll.h"
#include "ht.h"

#include <stdio.h>
#include <stdlib.h>

void lru_init(lru_cache *cache, size_t initial_capacity)
{
    dll_init(&cache->dll);
    ht_init(&cache->ht);
    cache->capacity = initial_capacity;
}

void lru_free(lru_cache *cache)
{
    // equiv to `dll_free()`
    node *cur = cache->dll.head;
    while (cur != NULL)
	{
        node *next = cur->next;
        free(cur->data);
        free(cur);
        cur = next;
    }

    ht_free(&cache->ht);
}

void *lru_get(lru_cache *cache, const char *key)
{
    node *result = ht_get(&cache->ht, key);
    if (result == NULL) return NULL;

    dll_move_to_head(&cache->dll, result);
    return result->data;
}

void lru_put(lru_cache *cache, const char *key, void *value)
{
    doubly_linked_list *dll = &cache->dll;

    node *n = ht_get(&cache->ht, key);
    if (n != NULL)
    {
        dll_move_to_head(dll, n);
        return;
    }

    if (dll->length >= cache->capacity)
    {
        n = dll->tail;
        dll_move_to_head(dll, n);
        ht_remove(&cache->ht, ((lru_data *)n->data)->key);
    }
    else
    {
        n = dll_prepend(dll, NULL);
        n->data = malloc(sizeof(lru_data));
        if (n->data == NULL)
        {
            fprintf(stderr, "could not allocate lru list data");
            exit(1);
        }
    }

    lru_data *data = n->data;
    data->key = key;
    data->value = value;
    ht_set(&cache->ht, key, n);
}
