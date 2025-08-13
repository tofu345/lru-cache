#include "dll.h"

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void exit_nomem(void) {
    fprintf(stderr, "out of memory\n");
}

bool cmp_str(void* a, void* b) {
    return strcmp((char*)a, (char*)b) == 0;
}

bool dll_is(dll* ll, char* expected[], int len) {
    int i;
    node* cur = ll->_head;
    for (i = 0; i < len && cur; i++) {
        if (!cmp_str(expected[i], cur->value))
            return false;
        cur = cur->next;
    }
    return i == len && cur == NULL;
}

static void test_dll_prepend(void) {
    dll* ll = dll_create();

    assert(dll_prepend(ll, "foo") != NULL);
    assert(dll_prepend(ll, "foo") != NULL);
    assert(dll_prepend(ll, "bar") != NULL);
    assert(dll_length(ll) == 3);

    char* expected[] = {
        "bar", "foo", "foo"
    };
    int len = sizeof(expected) / sizeof(expected[0]);
    assert(dll_is(ll, expected, len));

    dll_destroy(ll);
}

static void test_dll_append(void) {
    dll* ll = dll_create();

    assert(dll_append(ll, "foo") != NULL);
    assert(dll_append(ll, "foo") != NULL);
    assert(dll_append(ll, "bar") != NULL);
    assert(dll_length(ll) == 3);

    char* expected[] = {
        "foo", "foo", "bar"
    };
    int len = sizeof(expected) / sizeof(expected[0]);
    assert(dll_is(ll, expected, len));

    dll_destroy(ll);
}

static void test_dll_find_insert(void) {
    dll* ll = dll_create();

    assert(dll_append(ll, "foo") != NULL);
    assert(dll_append(ll, "baz") != NULL);
    assert(dll_append(ll, "baz") != NULL);
    assert(dll_append(ll, "bar") != NULL);

    node* baz1 = dll_find_from(ll->_head, "baz", cmp_str);
    assert(baz1 != NULL);
    assert(dll_insert_after(baz1, "bez") != NULL);
    assert(dll_insert_before(baz1, "beez") != NULL);

    char* expected[] = {
        "foo", "beez", "baz", "bez", "baz", "bar"
    };
    int len = sizeof(expected) / sizeof(expected[0]);
    assert(dll_is(ll, expected, len));

    dll_destroy(ll);
}

static void test_dll_pop(void) {
    dll* ll = dll_create();

    assert(dll_append(ll, "foo") != NULL);
    assert(dll_append(ll, "baz") != NULL);
    assert(dll_append(ll, "baz") != NULL);
    assert(dll_append(ll, "bar") != NULL);

    char* tail = dll_pop(ll);
    assert(tail != NULL && strcmp(tail, "bar") == 0);
    assert(dll_length(ll) == 3);
    assert(dll_pop(ll) != NULL);
    assert(dll_pop(ll) != NULL);
    assert(dll_pop(ll) != NULL);
    assert(dll_length(ll) == 0);

    dll_destroy(ll);
}

int main() {
    // my very own test suite :p
    struct test {
        char* name;
        void (*fn) (void);
    };
    struct test tests[] = {
        { "prepend", test_dll_prepend },
        { "append", test_dll_append },
        { "find_insert", test_dll_find_insert },
        { "pop", test_dll_pop },
    };
    size_t len = sizeof(tests) / sizeof(tests[0]);
    for (size_t i = 0; i < len; i++) {
        tests[i].fn();
        printf("%-30s ok\n", tests[i].name);
    }
    printf("all tests passed\n");
}
