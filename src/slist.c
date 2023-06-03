#define __S_SRC_FILE__
#include "slist.h"
#undef __S_SRC_FILE__

#include <stdlib.h>

typedef struct _snode {
    void*          element;
    struct _snode* next;
    struct _snode* prev;
} snode;

struct _slist {
    snode* front;
    snode* back;
};

struct _slsIter {
    const void* element;
    snode*      next;
};

slistIterator*
slist_iterator(slist* ls)
{
    if(ls != NULL && ls->front != NULL) {
        slistIterator* iterator = (slistIterator*) malloc(sizeof(slistIterator));
        if(iterator == NULL)
            return NULL;

        iterator->element = (const void*) ls->front->element;
        iterator->next    = ls->front->next;

        return iterator;
    }
    return NULL;
}

slistIterator*
slist_iterator_rev(slist* ls)
{
    if(ls != NULL && ls->back != NULL) {
        slistIterator* iterator = (slistIterator*) malloc(sizeof(slistIterator));
        if(iterator == NULL)
            return NULL;

        iterator->element = (const void*) ls->back->element;
        iterator->next    = ls->back->prev;

        return iterator;
    }
    return NULL;
}

const void*
slistiter_next(slistIterator* iterator)
{
    if(iterator != NULL) {
        if(iterator->element == NULL) {
            free(iterator);
            return NULL;
        }

        const void* temp = iterator->element;

        if(iterator->next != NULL) {
            iterator->element = iterator->next->element;
            iterator->next    = iterator->next->next;
        } else
            iterator->element = NULL;

        return temp;
    }
    return NULL;
}

const void*
slistiter_next_back(slistIterator* iterator)
{
    if(iterator != NULL) {
        if(iterator->element == NULL) {
            free(iterator);
            return NULL;
        }

        const void* temp = iterator->element;

        if(iterator->next != NULL) {
            iterator->element = iterator->next->element;
            iterator->next    = iterator->next->prev;
        } else
            iterator->element = NULL;

        return temp;
    }
    return NULL;
}

snode*
snode_new(void* element)
{
    snode* node = (snode*) malloc(sizeof(snode));
    if(node != NULL) {
        node->next    = NULL;
        node->prev    = NULL;
        node->element = element;
    }
    return node;
}

void*
snode_free(snode* node)
{
    void* temp = node->element;
    free(node);
    return temp;
}

slist*
slist_new(void)

{
    slist* ls = (slist*) malloc(sizeof(slist));
    if(ls != NULL) {
        ls->back  = NULL;
        ls->front = NULL;
    }
    return ls;
}

int
slist_insert_front(slist* ls, void* element)
{
    snode* new_front = snode_new(element);
    if(new_front == NULL)
        return 1;

    if(ls->front == NULL) {
        ls->back = new_front;
    } else {
        new_front->next = ls->front;
        ls->front->prev = new_front;
    }

    ls->front = new_front;

    return 0;
}

void*
slist_pop_front(slist* ls)
{
    if(ls->front == NULL)
        return NULL;

    snode* old_head = ls->front;
    ls->front       = old_head->next;

    if(ls->front != NULL)
        ls->front->prev = NULL;
    else
        ls->back = NULL;

    return snode_free(old_head);
}

void*
slist_pop_back(slist* ls)
{
    if(ls->back == NULL)
        return NULL;

    snode* old_tail = ls->back;
    ls->back        = old_tail->prev;

    if(ls->back != NULL)
        ls->back->next = NULL;
    else
        ls->front = NULL;

    return snode_free(old_tail);
}

/*
 * Returns NULL if slist or slist front node is NULL.
 * Returns element at the front of the list otherwise.
 */
const void*
slist_peek_front(slist* ls)
{
    // so ugly and short
    return (ls) ? (ls->front ? ls->front->element : NULL) : NULL;
}

/*
 * Cleans up the entire list and the values
 * if the 'free_fn' is provided.
 */
void
slist_free(slist* ls, FreeFn free_fn)
{
    void* element;
    while((element = slist_pop_front(ls)) != NULL)
        if(free_fn)
            free_fn(element);
}
