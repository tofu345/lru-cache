#include "dll.h"

#include <assert.h>
#include <stdlib.h>

dll* dll_create(void) {
    dll* l = malloc(sizeof(dll));
    if (l == NULL) return NULL;
    l->_head = NULL;
    l->_tail = NULL;
    return l;
}

void dll_destroy(dll* ll) {
    if (ll->_head == NULL) {
        assert(ll->_tail == NULL);
        free(ll);
        return;
    }

    node* cur = ll->_head;
    while (cur->next != NULL) {
        node* next = cur->next;
        free(cur);
        cur = next;
    }
    free(ll);
}

node* dll_head(dll* ll) {
    if (ll->_head == NULL) return NULL;
    while (ll->_head->prev != NULL) {
        ll->_head = ll->_head->prev;
    }
    return ll->_head;
}

node* dll_tail(dll* ll) {
    if (ll->_tail == NULL) return NULL;
    while (ll->_tail->next != NULL) {
        ll->_tail = ll->_tail->next;
    }
    return ll->_tail;
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
    if (ll->_head == NULL) {
        assert(ll->_tail == NULL);
        ll->_head = malloc(sizeof(node));
        if (ll->_head == NULL) return NULL;
        ll->_head->next = NULL;
        ll->_head->prev = NULL;
        ll->_head->value = value;

        ll->_tail = ll->_head;
        return ll->_head;
    }

    node* head = dll_insert_before(ll->_head, value);
    if (head == NULL) return NULL;
    ll->_head = head;
    return head;
}

node* dll_append(dll* ll, void* value) {
    if (ll->_tail == NULL) {
        assert(ll->_head == NULL);
        ll->_tail = malloc(sizeof(node));
        if (ll->_tail == NULL) return NULL;
        ll->_tail->next = NULL;
        ll->_tail->prev = NULL;
        ll->_tail->value = value;

        ll->_head = ll->_tail;
        return ll->_tail;
    }

    node* tail = dll_insert_after(ll->_tail, value);
    if (tail == NULL) {
        return NULL;
    }
    ll->_tail = tail;
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
    node* cur = ll->_head;
    while (cur != NULL) {
        len++;
        cur = cur->next;
    }
    return len;
}

void* dll_pop(dll* ll) {
    dll_tail(ll); // in case of stupidity
    if (ll->_tail == NULL) {
        assert(ll->_head == NULL);
        return NULL;
    }
    node* new_tail = ll->_tail->prev;
    void* val = ll->_tail->value;
    free(ll->_tail);
    if (new_tail == NULL) {
        ll->_head = new_tail;
    } else {
        new_tail->next = NULL;
    }
    ll->_tail = new_tail;
    return val;
}

dlli dll_iterator(dll* ll) {
    dlli it;
    it._node = ll->_head;
    return it;
}

bool dll_next(dlli* it) {
    if (it->_node == NULL) return false;
    it->value = it->_node->value;
    it->_node = it->_node->next;
    return true;
}
