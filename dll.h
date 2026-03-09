#ifndef DLL_H
#define DLL_H

#include <stdbool.h>
#include <stddef.h>

typedef struct node {
    void *value;
    struct node *prev;
    struct node *next;
} node;

typedef struct {
	// Best not to touch these manually.
    node *head, *tail;
} doubly_linked_list;

doubly_linked_list *dll_new(void);

void dll_free(doubly_linked_list *);

node *dll_prepend(doubly_linked_list *, void *value);

node *dll_append(doubly_linked_list *, void *value);

node *dll_find(doubly_linked_list *, void *value);

node *dll_insert_after(doubly_linked_list *, node *, void *value);

void dll_remove(doubly_linked_list *, node *);

#endif
