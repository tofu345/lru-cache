#include "dll.h"

#include <assert.h>
#include <stdlib.h>

doubly_linked_list *dll_new(void)
{
    return calloc(1, sizeof(doubly_linked_list));
}

void dll_free(doubly_linked_list *list)
{
    if (list->head == NULL)
	{
        assert(list->tail == NULL);
        free(list);
        return;
    }

    node *cur = list->head;
    while (cur != NULL)
	{
        node *next = cur->next;
        free(cur);
        cur = next;
    }
    free(list);
}

node *dll_prepend(doubly_linked_list *list, void *value)
{
    node *new = malloc(sizeof(node));
    if (new == NULL) return NULL;
    new->value = value;
	new->prev = NULL;

    if (list->head == NULL)
	{
        assert(list->tail == NULL);
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

node *dll_append(doubly_linked_list *list, void *value)
{
    node *new = malloc(sizeof(node));
    if (new == NULL) return NULL;
    new->value = value;
	new->next = NULL;

    if (list->tail == NULL)
	{
        assert(list->head == NULL);
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

node *dll_find(doubly_linked_list *list, void *value)
{
	node *cur = list->head;
	while (cur != NULL)
	{
		if (cur->value == value) return cur;
		cur = cur->next;
	}
	return NULL;
}

node *dll_insert_after(doubly_linked_list *list, node *n, void *value)
{
    assert(n != NULL && "cannot insert NULL node");

    node *new = malloc(sizeof(node));
    if (new == NULL) return NULL;
    new->value = value;
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

void dll_remove(doubly_linked_list *list, node *n)
{
    assert(n != NULL && "cannot remove NULL node");

	if (n->prev != NULL)
		n->prev->next = n->next;
	else
		list->head = n->next;

	if (n->next != NULL)
		n->next->prev = n->prev;
	else
		list->tail = n->prev;

    list->length--;
	free(n);
}
