/*
 * Name: Jorge Fernando Flores Pinto
 * ID: V00880059
 * SENG265, Summer 2017
 * linkedlist.c
 *
 * Based on the implementation approach described in "The Practice 
 * of Programming" by Kernighan and Pike (Addison-Wesley, 1999).
 *
 * Additional methods were added to this module, based on Mike's
 * slides on Dynamic Memory in C.
 */

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include "linkedlist.h"


void *emalloc(size_t n) {
    void *p;

    p = malloc(n);
    if (p == NULL) {
        fprintf(stderr, "malloc of %zu bytes failed", n);
        exit(1);
    }

    return p;
}


charval_t *new_charval(unsigned char c, int val) {
    charval_t *temp;

    temp = (charval_t *) emalloc(sizeof(charval_t));
    temp->c = c;
    temp->val = val;
    temp->next = NULL;


    return temp;
}
charval_t *new_char(int val) {
    charval_t *temp;

    temp = (charval_t *) emalloc(sizeof(charval_t));
    temp->c = '\0';
    temp->val = val;
    temp->next = NULL;


    return temp;
}
charval_t *find(charval_t *list, charval_t *cv) {
    for( ; list != NULL; list = list->next) {
        if (list->c == cv->c) {
            return list;
        }
    }
    //Otherwise, if value not found, return NULL.
    return NULL;
}
//Finds a character from a given key (val)
charval_t *find_char (charval_t *list, int key) {
    for( ; list != NULL; list = list->next) {
        if (list->val == key) {
            return list;
        }
    }
    //Otherwise, if value not found, return NULL.
    return NULL;
}

charval_t *add_front(charval_t *list, charval_t *cv) {
    if (list == NULL) {
        cv->next = list;
        return cv;
    }

    cv->next = list;
    return cv;
}


charval_t *add_end(charval_t *list, charval_t *cv) {
    charval_t *curr;

    if (list == NULL) {
        cv->next = NULL;
        return cv;
    }
    for (curr = list; curr->next != NULL; curr = curr->next);
    //for (curr = list; curr != NULL; curr = curr->next);
    curr->next = cv;
    //curr = cv;

    cv->next = NULL;
    return list;
}
charval_t *add_with_tail(charval_t *tail_pointer, charval_t *cv) {
    tail_pointer->next = cv;
    return cv;
}

charval_t *peek_front(charval_t *list) {
    return list;
}


charval_t *remove_front(charval_t *list) {
    if (list == NULL) {
        return NULL;
    }
    return list->next;
}
//Removes a specific node in the list, if found.
charval_t *remove_charval(charval_t *list, charval_t *cv) {
    charval_t *curr;
    charval_t *previous = NULL;
    int i = 0;
    for(curr = list; curr != NULL; curr = curr->next) {
        if(cv->c == curr->c) {
            if (previous == NULL && curr->next == NULL) {
                list = curr->next;
                //curr->tail = NULL;
                //curr->prev = NULL;
            }
            else if (previous == NULL) {
                list = curr->next;
                //list->tail = curr->tail;
                //list->prev = curr->prev;
            }
            else if(curr->next == NULL) {
                previous->next = curr->next;
                //previous->tail = previous;
                //curr->prev = previous;
                //list->tail = previous;
            }
            else {
                previous->next = curr->next;
                //(curr->next)->prev = previous;
                //prev->tail = (prev->next)->tail;

            }
            free_charval(curr);
            return list;
        }
        previous = curr;
        i++;
    }
    return NULL;
}


void free_charval(charval_t *cv) {
    assert(cv != NULL);
    free(cv);
}

void apply(charval_t *list,
           void (*fn)(charval_t *list, void *),
           void *arg)
{
    for ( ; list != NULL; list = list->next) {
        (*fn)(list, arg);
    }
}
void apply_until(charval_t *list, charval_t *cv,
           void (*fn)(charval_t *list))
{
    for ( ; list != cv; list = list->next) {
        (*fn)(list);
    }
}