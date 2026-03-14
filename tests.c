#include "dll.h"
#include "ht.h"
#include "lru.h"

#include <stdint.h>
#include <assert.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Lots of rooms for improvement in these tests...

const char foo[] = "foo";
const char bar[] = "bar";
const char baz[] = "baz";
const char fizzbuzz[] = "fizzbuzz";
const char buzzfizz[] = "buzzfizz";

static bool dll_is(doubly_linked_list *list, const char *expected[]);

static void test_dll_insertion(void)
{
    doubly_linked_list list;
    dll_init(&list);

    assert(dll_prepend(&list, (void *)foo) != NULL);
    assert(dll_prepend(&list, (void *)bar) != NULL);
    assert(dll_prepend(&list, (void *)baz) != NULL);

    assert(dll_is(&list, (const char *[]){ baz, bar, foo, NULL }));
    assert(list.length == (int)3);

    assert(dll_append(&list, (void *)bar) != NULL);
    assert(dll_append(&list, (void *)baz) != NULL);

    assert(dll_is(&list, (const char *[]){ baz, bar, foo, bar, baz, NULL }));
    assert(list.length == (int)5);

    node *fooNode = dll_find(&list, (void *)foo);
    assert(fooNode != NULL);
    assert(dll_insert_after(&list, fooNode, (void *)fizzbuzz) != NULL);
    assert(dll_is(&list, (const char *[]){ baz, bar, foo, fizzbuzz, bar, baz, NULL }));
    assert(list.length == (int)6);

    assert(dll_insert_after(&list, list.head, (void *)fizzbuzz) != NULL);
    assert(dll_is(&list, (const char *[]){ baz, fizzbuzz, bar, foo, fizzbuzz, bar, baz, NULL }));
    assert(list.length == (int)7);

    node *tail = dll_insert_after(&list, list.tail, (void *)buzzfizz);
    assert(tail != NULL);
    assert(dll_is(&list, (const char *[]){ baz, fizzbuzz, bar, foo, fizzbuzz, bar, baz, buzzfizz, NULL }));
    assert(tail == list.tail);
    assert(list.length == (int)8);

    dll_free(&list);
}

static void test_dll_removal(void)
{
    doubly_linked_list list;
    dll_init(&list);

    assert(dll_prepend(&list, (void *)foo) != NULL);
    assert(dll_prepend(&list, (void *)bar) != NULL);
    assert(dll_prepend(&list, (void *)baz) != NULL);
    assert(list.length == (int)3);

    node *bazNode = dll_find(&list, (void *)baz);
    assert(bazNode != NULL);

    dll_remove(&list, bazNode);

    assert(dll_is(&list, (const char *[]){ bar, foo, NULL }));
    assert(list.head->data == (void *)bar);
    assert(list.tail->data == (void *)foo);
    assert(list.length == (int)2);

    dll_remove(&list, list.head);

    assert(dll_is(&list, (const char *[]){ foo, NULL }));
    assert(list.head->data == (void *)foo);
    assert(list.tail->data == (void *)foo);
    assert(list.length == (int)1);

    dll_remove(&list, list.tail);

    assert(dll_is(&list, (const char *[]){ NULL }));
    assert(list.head == NULL);
    assert(list.tail == NULL);
    assert(list.length == (int)0);

    dll_free(&list);
}

static void test_dll_move_to_head(void)
{
    doubly_linked_list list;
    dll_init(&list);

    assert(dll_append(&list, (void *)foo) != NULL);
    assert(dll_append(&list, (void *)bar) != NULL);
    assert(dll_append(&list, (void *)baz) != NULL);
    assert(dll_append(&list, (void *)fizzbuzz) != NULL);
    assert(dll_append(&list, (void *)buzzfizz) != NULL);

    assert(dll_is(&list, (const char *[]){ foo, bar, baz, fizzbuzz, buzzfizz, NULL }));

    dll_move_to_head(&list, dll_find(&list, (void *)buzzfizz));
    assert(dll_is(&list, (const char *[]){ buzzfizz, foo, bar, baz, fizzbuzz, NULL }));

    dll_move_to_head(&list, dll_find(&list, (void *)fizzbuzz));
    assert(dll_is(&list, (const char *[]){ fizzbuzz, buzzfizz, foo, bar, baz, NULL }));

    dll_move_to_head(&list, dll_find(&list, (void *)foo));
    assert(dll_is(&list, (const char *[]){ foo, fizzbuzz, buzzfizz, bar, baz, NULL }));

    dll_free(&list);
}

static bool ht_has(hash_table *table, const char *expected[]);

static void test_ht_insertion(void)
{
    hash_table table;
    ht_init(&table);

    assert(ht_set(&table, foo, (void *)foo) != NULL);
    assert(ht_set(&table, bar, (void *)bar) != NULL);
    assert(ht_set(&table, baz, (void *)baz) != NULL);
    assert(ht_set(&table, fizzbuzz, (void *)fizzbuzz) != NULL);
    assert(ht_set(&table, buzzfizz, (void *)buzzfizz) != NULL);

    assert(ht_has(&table, (const char *[]){ foo, bar, baz, fizzbuzz, buzzfizz, NULL }));

    ht_free(&table);
}

static void test_ht_iteration(void)
{
    hash_table table;
    ht_init(&table);

    // could use some more.
    assert(ht_set(&table, foo,       (void *)1) != NULL);
    assert(ht_set(&table, bar,       (void *)2) != NULL);
    assert(ht_set(&table, baz,       (void *)3) != NULL);
    assert(ht_set(&table, fizzbuzz,  (void *)4) != NULL);
    assert(ht_set(&table, buzzfizz,  (void *)5) != NULL);

    int total = 0;
    const hash_table_entry *cur;
    hti it = ht_iterator(&table);
    while ((cur = ht_next(&it)))
    {
        total ^= (int)(uintptr_t)cur->value;
    }

    assert(total == (1 ^ 2 ^ 3 ^ 4 ^ 5));

    ht_free(&table);
}

static void test_ht_removal(void)
{
    hash_table table;
    ht_init(&table);

    assert(ht_set(&table, foo, (void *)foo) != NULL);
    assert(ht_set(&table, bar, (void *)bar) != NULL);
    assert(ht_set(&table, baz, (void *)baz) != NULL);
    assert(ht_set(&table, fizzbuzz, (void *)fizzbuzz) != NULL);
    assert(ht_set(&table, buzzfizz, (void *)buzzfizz) != NULL);

    ht_remove(&table, foo);
    assert(ht_has(&table, (const char *[]){ bar, baz, fizzbuzz, buzzfizz, NULL }));

    ht_remove(&table, fizzbuzz);
    assert(ht_has(&table, (const char *[]){ bar, baz, buzzfizz, NULL }));

    assert(ht_set(&table, foo, (void *)foo) != NULL);
    assert(ht_has(&table, (const char *[]){ foo, bar, baz, buzzfizz, NULL }));

    ht_remove(&table, buzzfizz);
    assert(ht_has(&table, (const char *[]){ foo, bar, baz, NULL }));

    ht_remove(&table, bar);
    assert(ht_has(&table, (const char *[]){ foo, baz, NULL }));

    assert(ht_set(&table, fizzbuzz, (void *)foo) != NULL);
    assert(ht_has(&table, (const char *[]){ foo, baz, fizzbuzz, NULL }));

    ht_remove(&table, baz);
    assert(ht_has(&table, (const char *[]){ foo, fizzbuzz, NULL }));

    ht_remove(&table, foo);
    assert(ht_has(&table, (const char *[]){ fizzbuzz, NULL }));

    ht_remove(&table, fizzbuzz);
    assert(ht_has(&table, (const char *[]){ NULL }));

    ht_free(&table);
}

static bool lru_dll_is(lru_cache *cache, const char *expected[]);

static void test_lru(void)
{
    lru_cache cache;
    lru_init(&cache, 3);

    lru_put(&cache, foo, (void *)foo);
    assert(lru_dll_is(&cache, (const char *[]){ foo, NULL }));
    assert(lru_get(&cache, foo) != NULL);

    lru_put(&cache, bar, (void *)bar);
    assert(lru_dll_is(&cache, (const char *[]){ bar, foo, NULL }));
    assert(lru_get(&cache, bar) != NULL);

    lru_put(&cache, baz, (void *)baz);
    assert(lru_dll_is(&cache, (const char *[]){ baz, bar, foo, NULL }));
    assert(lru_get(&cache, baz) != NULL);

    lru_put(&cache, fizzbuzz, (void *)fizzbuzz);
    assert(lru_dll_is(&cache, (const char *[]){ fizzbuzz, baz, bar, NULL }));
    assert(lru_get(&cache, foo) == NULL);
    assert(lru_get(&cache, fizzbuzz) != NULL);

    lru_put(&cache, baz, (void *)baz);
    assert(lru_dll_is(&cache, (const char *[]){ baz, fizzbuzz, bar, NULL }));
    assert(lru_get(&cache, baz) != NULL);

    lru_put(&cache, foo, (void *)foo);
    assert(lru_dll_is(&cache, (const char *[]){ foo, baz, fizzbuzz, NULL }));
    assert(lru_get(&cache, bar) == NULL);
    assert(lru_get(&cache, foo) != NULL);

    lru_free(&cache);
}

static bool ht_has(hash_table *table, const char *expected[])
{
    size_t i = 0;
    for (; *expected != NULL; expected++, i++)
    {
        if (ht_get(table, *expected) == NULL)
            return false;
    }
    return i == table->length;
}

static bool dll_is(doubly_linked_list *list, const char *expected[])
{
    node *cur = list->head, *prev = NULL;
    for (; *expected != NULL && cur != NULL; expected++, cur = cur->next)
    {
        if (strcmp(*expected, (char *)cur->data) != 0)
            return false;

        if (cur->prev != prev)
        {
            printf("cur->prev at %s is incorrect\n", (char *)cur->data);
            return false;
        }

        prev = cur;
    }
    return cur == NULL && *expected == NULL;
}

static bool lru_dll_is(lru_cache *cache, const char *expected[])
{
    node *cur = cache->dll.head, *prev = NULL;
    for (; *expected != NULL && cur != NULL; expected++, cur = cur->next)
    {
        if (strcmp(*expected, ((lru_data *)cur->data)->key) != 0)
            return false;

        if (cur->prev != prev)
        {
            printf("cur->prev at %s is incorrect\n", (char *)cur->data);
            return false;
        }

        prev = cur;
    }
    return cur == NULL && *expected == NULL;
}

int main()
{
    // my very own test suite :p
    struct test {
        char *name;
        void (*fn) (void);
    } tests[] = {
        { "dll_insertion",      test_dll_insertion },
        { "dll_removal",        test_dll_removal },
        { "dll_move_to_head",   test_dll_move_to_head },
        { "ht_insertion",       test_ht_insertion },
        { "ht_iteration",       test_ht_iteration },
        { "ht_removal",         test_ht_removal },
        { "lru",                test_lru },
        { NULL, NULL },
    };
    for (struct test *cur = tests; cur->name != NULL; cur++) {
        cur->fn();
        printf("%-17s ok\n", cur->name);
    }
}
