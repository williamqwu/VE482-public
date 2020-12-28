/**
 * @author Qinhang Wu (william_wu@sjtu.edu.cn)
 * @brief list utilities
 * @cdate 2020-10-18
 * @mdate 2020-11-08
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

/**
 * @brief initialize the list
 * 
 * @param list 
 */
void listConstructor(List *list); // initialize the list

/**
 * @brief append a new node at the end of the list
 * 
 * @param list 
 * @param node 
 */
void listAdd(List *list, Node *node); // add an element to the list

/**
 * @brief search for an element in the list with key;
 *      return NULL if the list is empty or key doesn't match
 * 
 * @param list 
 * @param key 
 * @return void* 
 */
void *listSearch(List *list, const char *key); // search for an element int the list

/**
 * @brief free the list
 * 
 * @param list 
 */
void listDestructor(List *list); // free the list

#endif // LIST_H
