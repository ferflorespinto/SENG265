/*
 * Some routines to show the linked list routines in action
 * (especially the way they are intended to be used). This works
 * with the *unmodified* linked-list routines.
 *
 * Note: This file is for demonstration purposes only. It will
 * not be used to evaluate student work.
 */
#include <stdio.h>
#include <stdlib.h>
#include "linkedlist.h"

void print_charval(charval_t *cv, void *arg) {
    printf((char *)arg,
        cv->c,
        cv->val
    );
}


void increment_count(charval_t *t, void *arg) {
    int *ip;
    ip = (int *)arg;
    (*ip)++;
}


void test01() {
    char *title = "test01";

    charval_t *list = NULL;
    charval_t *temp;
    unsigned char test_c[] = {'z', 't', 'a', 'c'};
    int test_val[] = {99, 5, 3, 42};
    unsigned char expected_c[] = {'a', 't', 'z', 'c'};
    int expected_val[] = {3, 5, 99, 42};
    int test_size = 4;
    int i;
    int len;

    for (i = 0; i < test_size-1; i++) {
        temp = new_charval(test_c[i], test_val[i]);
        list = add_front(list, temp);
    }

    temp = new_charval(test_c[test_size-1], test_val[test_size-1]);
    temp = add_end(list, temp);

    apply(list, print_charval, "('%c' %d) ");
    printf("\n");

    len = 0;
    apply(list, increment_count, &len);
    printf("%s -- length: %d\n", title,len);

    for (i = 0; i < test_size; i++) {
        temp = peek_front(list);
        if (temp->c != expected_c[i] || temp->val != expected_val[i]) {
            fprintf(stderr, "%s: expected ('%c' %d), " \
                "got instead ('%c' %d)\n",
                title, expected_c[i], expected_val[i],
                temp->c, temp->val
            );
            fprintf(stdout, "%s: FAILED\n", title);
            return;
        }
        list = remove_front(list);
        free_charval(temp);
    }

    fprintf(stdout, "%s: passed\n", title);

    return;
}
void find_test() {
    char *title = "find_test";

    charval_t *list = NULL;
    charval_t *temp;
    charval_t *found_item;
    unsigned char test_c[] = {'z', 't', 'a', 'c'};
    int test_val[] = {99, 5, 3, 42};
    unsigned char dummy[] = {'c', 'w', 'z', 't', 'a', 'r', 'y'};
    int dummy_val[] = {1, 2, 3, 4, 5, 6, 7};

   // int expected_val[] = {3, 5, 99, 42};
    int test_size = 4;
    int dummy_size = 7;
    int i;
    int len;
    int find_val = 1;

    for (i = 0; i < test_size-1; i++) {
        temp = new_charval(test_c[i], test_val[i]);
        list = add_front(list, temp);
    }

    temp = new_charval(test_c[test_size-1], test_val[test_size-1]);
    temp = add_end(list, temp);

    apply(list, print_charval, "('%c' %d) ");
    printf("\n");

    len = 0;
    apply(list, increment_count, &len);
    printf("%s -- length: %d\n", title,len);

    temp = new_charval(dummy[find_val], dummy_val[find_val]);
    found_item = find(list, temp);

    for (i = 0; i < dummy_size; i++) {
        temp = new_charval(dummy[i], dummy_val[i]);
        found_item = find(list, temp);
        if (found_item != NULL) {
            fprintf(stdout, "found: ('%c' %d)\n", found_item->c, found_item->val);
            fprintf(stdout, "%s: passed\n", title);

        }
        else {
            fprintf(stderr, "Test failed. Node ('%c' %d) was not found.\n", temp->c, temp->val);
        }
    }

    return;
}
void delete_test() {
    char *title = "delete_test";
    //charval_t *head = NULL;
    charval_t *tailp = NULL;

    charval_t *list = NULL;
    charval_t *list_temp = NULL;
    charval_t *temp;
    unsigned char test_c[] = {'z', 't', 'a', 'c'};
    int test_val[] = {99, 5, 3, 42};
    unsigned char dummy[] = {'c', 'w', 'z', 't', 'a', 'r', 'y'};
    int dummy_val[] = {1, 2, 3, 4, 5, 6, 7};

    int test_size = 4;
    int dummy_size = 7;
    int i;
    int len;
    //charval_t *tailpointer;
    printf("%s\n", title);

    tailp = new_charval(test_c[0], test_val[0]);
    list = add_end(list, tailp);
    tailp = list;
    //head = list;
    //head->tail = list;
    apply(list, print_charval, "('%c' %d) ");
    printf("\n");
    for (i = 1; i < test_size; i++) {
        temp = new_charval(test_c[i], test_val[i]);
        tailp = add_with_tail(tailp, temp);
        //tailp = temp;
        //head->tail = tailp;
    }

    //temp = new_charval(test_c[test_size-1], test_val[test_size-1]);
    //temp = add_end(list, temp);

    apply(list, print_charval, "('%c' %d) ");
    printf("\n");

    apply(tailp, print_charval, "('%c' %d) ");
    printf("\n");

    /*len = 0;
    apply(list, increment_count, &len);
    printf("%s -- length: %d\n", title,len);

    temp = new_charval(dummy[0], dummy_val[0]);
    list_temp = list;*/
    /*for (i = 0; i < dummy_size; i++) {
        if (list != NULL) {
            printf("problem is in printing\n");
            tailpointer = list->tail;
            printf("tail of list: ('%c' %d)\n", tailpointer->c, tailpointer->val);
            apply(list, print_charval, "('%c' %d) ");
            printf("\n");
        }
        else {
           //printf("tail of list: ('%c' %d)\n", tailpointer->c, tailpointer->val);
            printf("List is empty.\n");
            break;
        }
        printf("Trying to remove ('%c' %d) from list\n", dummy[i], dummy_val[i]);
        temp = new_charval(dummy[i], dummy_val[i]);
        list_temp = remove_charval(list, temp);
        if (list->next == list_temp) {
            printf("gotta update the head\n");
            list = list_temp;
        }
        if (list_temp != NULL || list == NULL) {
            fprintf(stdout, "Deletion successful\n");

        }
        else {
            fprintf(stderr, "Test failed. Node ('%c' %d) was not in list.\n", temp->c, temp->val);
        }
        
    }
    temp = new_charval(dummy[6], dummy_val[6]);
    list = add_end(list, temp);
    tailpointer = list->tail;
    printf("tail of list: ('%c' %d)\n", tailpointer->c, tailpointer->val);
    apply(list, print_charval, "('%c' %d) ");
    printf("\n");

    temp = new_charval(dummy[5], dummy_val[5]);
    list = add_end(list, temp);
    tailpointer = list->tail;
    printf("tail of list: ('%c' %d)\n", tailpointer->c, tailpointer->val);
    apply(list, print_charval, "('%c' %d) ");
    printf("\n");

    temp = new_charval(dummy[4], dummy_val[4]);
    list = add_front(list, temp);
    tailpointer = list->tail;
    printf("tail of list: ('%c' %d)\n", tailpointer->c, tailpointer->val);
    apply(list, print_charval, "('%c' %d) ");
    printf("\n");*/

    return;
}


int main(int argc, char *argv[]) {
    test01();
    printf("--------------\n");
    find_test();
    printf("--------------\n");
    delete_test();

    return (0);
}