#include "dll.h"

#include <assert.h>
#include <stdlib.h>

void dll_init(doubly_linked_list *list)
{
    *list = (doubly_linked_list){0};
}

void dll_free(doubly_linked_list *list)
{
    node *cur = list->head;
    while (cur != NULL)
	{
        node *next = cur->next;
        free(cur);
        cur = next;
    }
}

node *dll_prepend(doubly_linked_list *list, void *data)
{
    node *new = malloc(sizeof(node));
    if (new == NULL) return NULL;
    new->data = data;
	new->prev = NULL;

    if (list->head == NULL)
	{
		new->next = NULL;
        list->tail = new;
    }
	else
	{
		new->next = list->head;
		list->head->prev = new;
	}

	list->head = new;
    list->length++;
	return new;
}

node *dll_append(doubly_linked_list *list, void *data)
{
    node *new = malloc(sizeof(node));
    if (new == NULL) return NULL;
    new->data = data;
	new->next = NULL;

    if (list->tail == NULL)
	{
		new->prev = NULL;
		list->head = new;
    }
	else
	{
		new->prev = list->tail;
		list->tail->next = new;
	}

	list->tail = new;
    list->length++;
	return new;
}

node *dll_find(doubly_linked_list *list, void *data)
{
	node *cur = list->head;
	while (cur != NULL)
	{
		if (cur->data == data) return cur;
		cur = cur->next;
	}
	return NULL;
}

node *dll_insert_after(doubly_linked_list *list, node *n, void *data)
{
    assert(n != NULL && "cannot insert NULL node");

    node *new = malloc(sizeof(node));
    if (new == NULL) return NULL;
    new->data = data;
	new->prev = n;
	new->next = n->next;

	if (n->next != NULL)
		n->next->prev = new;
	else
		list->tail = new;

	n->next = new;
    list->length++;
	return new;
}

static void _dll_remove(doubly_linked_list *list, node *n)
{
	if (n->prev != NULL)
		n->prev->next = n->next;
	else
		list->head = n->next;

	if (n->next != NULL)
		n->next->prev = n->prev;
	else
		list->tail = n->prev;
}

void dll_remove(doubly_linked_list *list, node *n)
{
    assert(n != NULL && "cannot remove NULL node");

    _dll_remove(list, n);
	free(n);
    list->length--;
}

void dll_move_to_head(doubly_linked_list *list, node *n)
{
    assert(n != NULL && "cannot remove NULL node");
    assert(list->head != NULL && "cannot move to NULL head");

    if (n == list->head) return;

    _dll_remove(list, n);
    n->next = list->head;
    n->prev = NULL;
    list->head->prev = n;
    list->head = n;
}
