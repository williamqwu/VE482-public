/**
 * @author Qinhang Wu (william_wu@sjtu.edu.cn)
 * @brief VE482 l5 ex1
 * @cdate 2020-10-27
 */

#ifndef UI_H
#define UI_H

#include "list.h"
#include "sort.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#define MAX_KEY 1024
#define MAX_STR 1024

#define DELIM "_."

void getType(Type *type, char *argv[]);

void insert(List *list, char *key, void *data);

/**
 * @brief print the list in order
 * 
 * @param type 
 * @param list 
 */
void print(Type *type, const List *list);

void ui_cmd(Type *type, char *argv[]);

void ui_menu();

// TODO: add comments

#endif // UI_H
