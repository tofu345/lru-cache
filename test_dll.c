#include "dll.h"

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

void fail(const char *format, ...)
{
	va_list args;
	va_start(args, format);
	vfprintf(stderr, format, args);
	va_end(args);
	exit(1);
}

bool dll_is(doubly_linked_list *list, const char *expected[])
{
    node *cur = list->head;
    for (; *expected != NULL && cur != NULL; expected++, cur = cur->next)
	{
        if (strcmp(*expected, (char *)cur->value) != 0)
            return false;
    }
	return cur == NULL && *expected == NULL;
}

static void test_dll_insertion(void)
{
	doubly_linked_list *list = dll_new();

    assert(dll_prepend(list, "foo") != NULL);
    assert(dll_prepend(list, "bar") != NULL);
    assert(dll_prepend(list, "baz") != NULL);

    assert(dll_is(list, (const char *[]){ "baz", "bar", "foo", NULL }));

    assert(dll_append(list, "bar") != NULL);
    assert(dll_append(list, "baz") != NULL);

    assert(dll_is(list, (const char *[]){ "baz", "bar", "foo", "bar", "baz", NULL }));

	dll_insert_after(list, list->head, "fizzbuzz");
    assert(dll_is(list, (const char *[]){ "baz", "fizzbuzz", "bar", "foo", "bar", "baz", NULL }));

	node *tail = dll_insert_after(list, list->tail, "buzzfizz");
    assert(dll_is(list, (const char *[]){ "baz", "fizzbuzz", "bar", "foo", "bar", "baz", "buzzfizz", NULL }));
	assert(tail == list->tail);

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

	node *baz = dll_find(list, (void *)values[2]);
	assert(baz != NULL);

	dll_remove(list, baz);

    assert(dll_is(list, (const char *[]){ "foo", "bar", NULL }));
	assert(list->head->value == (void *)values[0]);
	assert(list->tail->value == (void *)values[1]);

	dll_remove(list, list->head);

    assert(dll_is(list, (const char *[]){ "bar", NULL }));
	assert(list->head->value == (void *)values[1]);
	assert(list->tail->value == (void *)values[1]);

	dll_remove(list, list->tail);

    assert(dll_is(list, (const char *[]){ NULL }));
	assert(list->head == NULL);
	assert(list->tail == NULL);

    dll_free(list);
}

int main()
{
    // my very own test suite :p
    struct test {
        char *name;
        void (*fn) (void);
    } tests[] = {
        { "insertion", test_dll_insertion },
        { "removal", test_dll_removal },
		{ NULL, NULL },
    };
    for (struct test *cur = tests; cur->name != NULL; cur++) {
        cur->fn();
        printf("%-13s ok\n", cur->name);
    }
    printf("all tests passed\n");
}
