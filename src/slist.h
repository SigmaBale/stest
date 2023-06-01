#ifndef __S_LIST_H__
#define __S_LIST_H__

#if !defined(__S_HEADER__) && !defined(__S_SRC_FILE__)
#error "Private header, only <stest.h> can be included directly."
#endif

typedef void (*FreeFn)(void *);

typedef struct _slist slist;

typedef struct _slsIter slistIterator;

slist *slist_new(void);

int slist_insert_front(slist *ls, void *element);

void *slist_pop_front(slist *ls);

void *slist_pop_back(slist *ls);

void slist_free(slist *list, FreeFn free_fn);

slistIterator *slist_iterator(slist *ls);

slistIterator *slist_iterator_rev(slist *ls);

const void *slistiter_next(slistIterator *iterator);

const void *slistiter_next_back(slistIterator *iterator);

#endif
