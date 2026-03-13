#ifndef DOUBLY_LINKED_LIST_H
#define DOUBLY_LINKED_LIST_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

typedef struct node {
    void *data;
    struct node *prev;
    struct node *next;
} node;

typedef struct {
    // Best not to change these manually.
    node *head, *tail;
    uint64_t length;
} doubly_linked_list;

void dll_init(doubly_linked_list *);

void dll_free(doubly_linked_list *);

node *dll_prepend(doubly_linked_list *, void *data);

node *dll_append(doubly_linked_list *, void *data);

node *dll_find(doubly_linked_list *, void *data);

node *dll_insert_after(doubly_linked_list *, node *, void *data);

void dll_remove(doubly_linked_list *, node *);

void dll_move_to_head(doubly_linked_list *list, node *);

#endif // DOUBLY_LINKED_LIST_H
