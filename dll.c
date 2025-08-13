#include "dll.h"

#include <assert.h>
#include <stdlib.h>

dll* dll_create(void) {
    dll* l = malloc(sizeof(dll));
    if (l == NULL) return NULL;
    l->head = NULL;
    l->tail = NULL;
    return l;
}

void dll_destroy(dll* ll) {
    if (ll->head == NULL) {
        assert(ll->tail == NULL);
        free(ll);
        return;
    }

    node* cur = ll->head;
    while (cur->next != NULL) {
        node* next = cur->next;
        free(cur);
        cur = next;
    }
    free(ll);
}

node* dll_insert_before(node* n, void* val) {
    assert(n != NULL);
    node* new = malloc(sizeof(node));
    if (new == NULL) return NULL;
    new->value = val;
    new->prev = n->prev;
    new->next = n;
    if (n->prev != NULL)
        n->prev->next = new;
    n->prev = new;
    return new;
}

node* dll_insert_after(node* n, void* val) {
    assert(n != NULL);
    node* new = malloc(sizeof(node));
    if (new == NULL) return NULL;
    new->value = val;
    new->next = n->next;
    new->prev = n;
    if (n->next != NULL)
        n->next->prev = new;
    n->next = new;
    return new;
}

node* dll_prepend(dll* ll, void* value) {
    if (ll->head == NULL) {
        assert(ll->tail == NULL);
        ll->head = malloc(sizeof(node));
        if (ll->head == NULL) return NULL;
        ll->head->next = NULL;
        ll->head->prev = NULL;
        ll->head->value = value;

        ll->tail = ll->head;
        return ll->head;
    }

    node* head = dll_insert_before(ll->head, value);
    if (head == NULL) return NULL;
    ll->head = head;
    return head;
}

node* dll_append(dll* ll, void* value) {
    if (ll->tail == NULL) {
        assert(ll->head == NULL);
        ll->tail = malloc(sizeof(node));
        if (ll->tail == NULL) return NULL;
        ll->tail->next = NULL;
        ll->tail->prev = NULL;
        ll->tail->value = value;

        ll->head = ll->tail;
        return ll->tail;
    }

    node* tail = dll_insert_after(ll->tail, value);
    if (tail == NULL) {
        return NULL;
    }
    ll->tail = tail;
    return tail;
}

node* dll_find_from(node* cur, void* value, bool (*cmp) (void*, void*)) {
    while (cur != NULL) {
        if (cmp(cur->value, value)) {
            return cur;
        }
        cur = cur->next;
    }
    return NULL;
}

size_t dll_length(dll* ll) {
    size_t len = 0;
    node* cur = ll->head;
    while (cur != NULL) {
        len++;
        cur = cur->next;
    }
    return len;
}

dlli dll_iterator(dll* ll) {
    dlli it;
    it._node = ll->head;
    return it;
}

bool dll_next(dlli* it) {
    if (it->_node == NULL) return false;
    it->value = it->_node->value;
    it->_node = it->_node->next;
    return true;
}
