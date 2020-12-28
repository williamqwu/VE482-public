#ifndef LIST_H
#define LIST_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include <time.h>

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

// void listSort(); // sort the list w.r.t. instruction

// void listPrint(const List *list); // print the elements in the list

void listDestructor(List *list); // free the list

#endif // LIST_H
