#ifndef DLL_H
#define DLL_H

#include <stdbool.h>
#include <stddef.h>

// https://stackoverflow.com/a/2672067
//
// "It's C after all, if people want to screw up, they should be allowed to" i agree
typedef struct node node;
struct node {
    void* value;
    node* prev;
    node* next;
};

// Doubly Linked List
typedef struct {
    // dont touch this, use `dll_head`
    node* _head;
    // dont touch this, use `dll_tail`
    node* _tail;
} dll;

dll* dll_create(void);

// Free all allocated nodes, but not values.
void dll_destroy(dll* ll);

// Retrieve head of dll. This is provided in case operations individual nodes
// leave `ll._head` incorrect
node* dll_head(dll* ll);

// Retrieve tail of dll. This is provided in case operations individual nodes
// leave `ll._tail` incorrect
node* dll_tail(dll* ll);

// Prepend node with `value` to head of dll. Returns NULL if out of memory
node* dll_prepend(dll* ll, void* value);

// Append node with `value` to tail of dll. Returns NULL if out of memory
node* dll_append(dll* ll, void* value);

// Drop tail of dll. Returns NULL if empty
void* dll_pop(dll* ll);

// Find node from `cur` with value `value`, equality tested with `cmp`.
// Returns NULL if not found
node* dll_find_from(node* cur, void* value, bool (*cmp) (void*, void*));

// Insert node with `value` to before node `n`. Returns NULL if out of memory
node* dll_insert_before(node* n, void* val);

// Insert node with `value` to after node `n`. Returns NULL if out of memory
node* dll_insert_after(node* n, void* val);

size_t dll_length(dll* ll);

// not necessary but for the sake of convenience
typedef struct {
    void* value;

    // Don't use change this field directly.
    node* _node;
} dlli;

// Return new dll iterator (for use with dll_next).
dlli dll_iterator(dll* ll);

// Move iterator to next item in dll, update iterator's value to current item,
// and return true. If there are no more items, return false. Don't modify the
// dll during iteration.
bool dll_next(dlli* it);

#endif
