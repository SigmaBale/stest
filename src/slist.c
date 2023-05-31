#define __S_SRC_FILE__
#include "slist.h"
#undef __S_SRC_FILE__

#include <stdlib.h>

struct _slist {
    snode* front;
    snode* back;
};

struct _snode {
    void*  element;
    snode* next;
    snode* prev;
};

struct _slistiterator {
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

const void*
slistiter_next(slistIterator* iterator)
{
    if(iterator != NULL) {
        if(iterator->element == NULL) {
            free(iterator);
            return NULL;
        }

        const void* temp = iterator->element;

        if(iterator->next != NULL)
            iterator->element = iterator->next->element;
        else
            iterator->element = NULL;

        iterator->next = iterator->next->next;

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

int
slist_remove_front(slist* ls, FreeFn free_fn)
{
    if(ls->front == NULL)
        return 1;

    snode* old_head = ls->front;
    ls->front       = old_head->next;

    if(ls->front != NULL)
        ls->front->prev = NULL;
    else
        ls->back = NULL;

    if(free_fn)
        free_fn(snode_free(old_head));

    return 0;
}

int
slist_remove_back(slist* ls, FreeFn free_fn)
{
    if(ls->back == NULL)
        return 1;

    snode* old_tail = ls->back;
    ls->back        = old_tail->prev;

    if(ls->back != NULL)
        ls->back->next = NULL;
    else
        ls->front = NULL;

    if(free_fn)
        free_fn(snode_free(old_tail));

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

void
slist_free(slist* ls, FreeFn free_fn)
{
    while(slist_remove_front(ls, free_fn) == 0)
        ;
}
