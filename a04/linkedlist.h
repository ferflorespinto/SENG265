#ifndef _LINKEDLIST_H_
#define _LINKEDLIST_H_

typedef struct charval charval_t;

struct charval {
    unsigned char c;
    int           val;
    charval_t *next;
};

charval_t *new_charval(unsigned char c, int val);
charval_t *new_char(int val);
charval_t *find(charval_t *, charval_t *);
charval_t *find_char (charval_t *, int key);
charval_t *add_front(charval_t *, charval_t *);
charval_t *add_end(charval_t *, charval_t *);
charval_t *add_with_tail(charval_t *, charval_t *);
charval_t *peek_front(charval_t *);
charval_t *remove_front(charval_t *);
charval_t *remove_charval(charval_t *, charval_t *);
void free_charval(charval_t *);

void       apply(charval_t *, void(*fn)(charval_t *, void *), void *arg);
void 	   apply_until(charval_t *, charval_t *, void (*fn)(charval_t *));
#endif