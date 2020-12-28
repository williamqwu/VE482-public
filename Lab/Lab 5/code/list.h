/**
 * @author Qinhang Wu (william_wu@sjtu.edu.cn)
 * @brief VE482 l5 ex1
 * @cdate 2020-10-18
 * @mdate 2020-10-27
 */

#ifndef LIST_H
#define LIST_H

#include "stdio.h"
#include "string.h"
#include "stdlib.h" // free

typedef struct _Node{
    char *key;
    void *data;
    struct _Node *next;
} Node;

typedef struct _List{
    Node *head;
    int length;
} List;

void listConstructor(List *list); // initialize the list

void listAdd(List *list, Node *node); // add an element to the list

void *listSearch(List *list, const char *key); // search for an element int the list

void listDestructor(List *list); // free the list

#endif // LIST_H
