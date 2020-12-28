/**
 * @author Qinhang Wu (william_wu@sjtu.edu.cn)
 * @brief user interface for csv-plugin
 * @cdate 2020-11-08
 */

#ifndef UI_H
#define UI_H

#include "../src-kernel/plugin-api.h"
#include "sort.h"

#define MAX_KEY 1024
#define MAX_STR 1024

#define DELIM "_."

void insert(List *list, char *key, void *data);

/**
 * @brief print the list in order
 * 
 * @param type 
 * @param list 
 */
// void print(Type *type, const List *list);

// void ui_menu();

#endif // UI_H
