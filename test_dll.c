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
    node* cur = ll->head;
    for (i = 0; i < len && cur; i++) {
        if (!cmp_str(expected[i], cur->value))
            return false;
        cur = cur->next;
    }
    return i == len;
}

static void test_dll_prepend(void) {
    dll* ll = dll_create();

    assert(dll_prepend(ll, "foo") != NULL);
    assert(dll_prepend(ll, "foo") != NULL);
    assert(dll_prepend(ll, "bar") != NULL);
    assert(dll_length(ll) == 3);

    char* elems[] = {
        "bar", "foo", "foo"
    };
    int len = sizeof(elems) / sizeof(elems[0]);
    assert(dll_is(ll, elems, len));

    dll_destroy(ll);
    printf("test_dll_prepend: ok\n");
}

static void test_dll_append(void) {
    dll* ll = dll_create();

    assert(dll_append(ll, "foo") != NULL);
    assert(dll_append(ll, "foo") != NULL);
    assert(dll_append(ll, "bar") != NULL);
    assert(dll_length(ll) == 3);

    char* elems[] = {
        "foo", "foo", "bar"
    };
    int len = sizeof(elems) / sizeof(elems[0]);
    assert(dll_is(ll, elems, len));

    dll_destroy(ll);
    printf("test_dll_append: ok\n");
}

static void test_dll_find_insert(void) {
    dll* ll = dll_create();

    assert(dll_append(ll, "foo") != NULL);
    assert(dll_append(ll, "baz") != NULL);
    assert(dll_append(ll, "baz") != NULL);
    assert(dll_append(ll, "bar") != NULL);

    node* baz1 = dll_find_from(ll->head, "baz", cmp_str);
    assert(baz1 != NULL);
    assert(dll_insert_after(baz1, "bez") != NULL);
    assert(dll_insert_before(baz1, "beez") != NULL);

    char* elems[] = {
        "foo", "beez", "baz", "bez", "baz", "bar"
    };
    int len = sizeof(elems) / sizeof(elems[0]);
    assert(dll_is(ll, elems, len));

    dll_destroy(ll);
    printf("test_dll_find_insert: ok\n");
}

int main() {
    void (*tests[]) (void) = {
        test_dll_prepend,
        test_dll_append,
        test_dll_find_insert,
    };
    size_t len = sizeof(tests) / sizeof(tests[0]);
    for (size_t i = 0; i < len; i++) {
        tests[i]();
    }
    printf("all tests passed\n");
}
