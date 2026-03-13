#include "dll.h"
#include "ht.h"

#include <stdint.h>
#include <assert.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

const char foo[] = "foo";
const char bar[] = "bar";
const char baz[] = "baz";
const char fizzbuzz[] = "fizzbuzz";
const char buzzfizz[] = "buzzfizz";

static bool dll_is(doubly_linked_list *list, const char *expected[]);
static bool ht_has(hash_table *table, const char *expected[]);

static void test_dll_insertion(void)
{
    doubly_linked_list *list = dll_new();

    assert(dll_prepend(list, (void *)foo) != NULL);
    assert(dll_prepend(list, (void *)bar) != NULL);
    assert(dll_prepend(list, (void *)baz) != NULL);

    assert(dll_is(list, (const char *[]){ baz, bar, foo, NULL }));
    assert(list->length == 3);

    assert(dll_append(list, (void *)bar) != NULL);
    assert(dll_append(list, (void *)baz) != NULL);

    assert(dll_is(list, (const char *[]){ baz, bar, foo, bar, baz, NULL }));
    assert(list->length == 5);

    node *fooNode = dll_find(list, (void *)foo);
    assert(fooNode != NULL);
    assert(dll_insert_after(list, fooNode, (void *)fizzbuzz) != NULL);
    assert(dll_is(list, (const char *[]){ baz, bar, foo, fizzbuzz, bar, baz, NULL }));
    assert(list->length == 6);

    assert(dll_insert_after(list, list->head, (void *)fizzbuzz) != NULL);
    assert(dll_is(list, (const char *[]){ baz, fizzbuzz, bar, foo, fizzbuzz, bar, baz, NULL }));
    assert(list->length == 7);

    node *tail = dll_insert_after(list, list->tail, (void *)buzzfizz);
    assert(tail != NULL);
    assert(dll_is(list, (const char *[]){ baz, fizzbuzz, bar, foo, fizzbuzz, bar, baz, buzzfizz, NULL }));
    assert(tail == list->tail);
    assert(list->length == 8);

    dll_free(list);
}

static void test_dll_removal(void)
{
    doubly_linked_list *list = dll_new();

    const char *values[] = { "foo", "bar", "baz", NULL };
    for (const char **cur = values; *cur != NULL; cur++)
    {
        assert(dll_append(list, (void *)(*cur)) != NULL);
    }
    assert(list->length == 3);

    node *baz = dll_find(list, (void *)values[2]);
    assert(baz != NULL);

    dll_remove(list, baz);

    assert(dll_is(list, (const char *[]){ "foo", "bar", NULL }));
    assert(list->head->value == (void *)values[0]);
    assert(list->tail->value == (void *)values[1]);
    assert(list->length == 2);

    dll_remove(list, list->head);

    assert(dll_is(list, (const char *[]){ "bar", NULL }));
    assert(list->head->value == (void *)values[1]);
    assert(list->tail->value == (void *)values[1]);
    assert(list->length == 1);

    dll_remove(list, list->tail);

    assert(dll_is(list, (const char *[]){ NULL }));
    assert(list->head == NULL);
    assert(list->tail == NULL);
    assert(list->length == 0);

    dll_free(list);
}

static void ht_insertion(void)
{
    hash_table *table = ht_new();

    assert(ht_set(table, foo, (void *)foo) != NULL);
    assert(ht_set(table, bar, (void *)bar) != NULL);
    assert(ht_set(table, baz, (void *)baz) != NULL);
    assert(ht_set(table, fizzbuzz, (void *)fizzbuzz) != NULL);
    assert(ht_set(table, buzzfizz, (void *)buzzfizz) != NULL);

    assert(ht_has(table, (const char *[]){ foo, bar, baz, fizzbuzz, buzzfizz, NULL }));

    ht_free(table);
}

static void ht_iteration(void)
{
    hash_table *table = ht_new();

    // could use some more.
    assert(ht_set(table, foo,       (void *)1) != NULL);
    assert(ht_set(table, bar,       (void *)2) != NULL);
    assert(ht_set(table, baz,       (void *)3) != NULL);
    assert(ht_set(table, fizzbuzz,  (void *)4) != NULL);
    assert(ht_set(table, buzzfizz,  (void *)5) != NULL);

    int total = 0;
    const hash_table_entry *cur;
    hti it = ht_iterator(table);
    while ((cur = ht_next(&it)))
    {
        total ^= (int)(uintptr_t)cur->value;
    }

    assert(total == (1 ^ 2 ^ 3 ^ 4 ^ 5));

    ht_free(table);
}

static void ht_removal(void)
{
    hash_table *table = ht_new();

    assert(ht_set(table, foo, (void *)foo) != NULL);
    assert(ht_set(table, bar, (void *)bar) != NULL);
    assert(ht_set(table, baz, (void *)baz) != NULL);
    assert(ht_set(table, fizzbuzz, (void *)fizzbuzz) != NULL);
    assert(ht_set(table, buzzfizz, (void *)buzzfizz) != NULL);

    ht_remove(table, foo);
    assert(ht_has(table, (const char *[]){ bar, baz, fizzbuzz, buzzfizz, NULL }));

    ht_remove(table, fizzbuzz);
    assert(ht_has(table, (const char *[]){ bar, baz, buzzfizz, NULL }));

    assert(ht_set(table, foo, (void *)foo) != NULL);
    assert(ht_has(table, (const char *[]){ foo, bar, baz, buzzfizz, NULL }));

    ht_remove(table, buzzfizz);
    assert(ht_has(table, (const char *[]){ foo, bar, baz, NULL }));

    ht_remove(table, bar);
    assert(ht_has(table, (const char *[]){ foo, baz, NULL }));

    assert(ht_set(table, fizzbuzz, (void *)foo) != NULL);
    assert(ht_has(table, (const char *[]){ foo, baz, fizzbuzz, NULL }));

    ht_remove(table, baz);
    assert(ht_has(table, (const char *[]){ foo, fizzbuzz, NULL }));

    ht_remove(table, foo);
    assert(ht_has(table, (const char *[]){ fizzbuzz, NULL }));

    ht_remove(table, fizzbuzz);
    assert(ht_has(table, (const char *[]){ NULL }));

    ht_free(table);
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
        if (strcmp(*expected, (char *)cur->value) != 0)
            return false;

        if (cur->prev != prev)
        {
            printf("cur->prev at %s is incorrect\n", (char *)cur->value);
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
        { "dll_insertion",  test_dll_insertion },
        { "dll_removal",    test_dll_removal },
        { "ht_insertion",   ht_insertion },
        { "ht_iteration",   ht_iteration },
        { "ht_removal",     ht_removal },
        { NULL, NULL },
    };
    for (struct test *cur = tests; cur->name != NULL; cur++) {
        cur->fn();
        printf("%-15s ok\n", cur->name);
    }
}
