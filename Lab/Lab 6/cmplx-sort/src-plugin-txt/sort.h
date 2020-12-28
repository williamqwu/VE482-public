/**
 * @author Qinhang Wu (william_wu@sjtu.edu.cn)
 * @brief sorting utilities for txt-plugin
 * @cdate 2020-10-27
 * @mdate 2020-11-08
 */

#ifndef SORT_H
#define SORT_H

#include "../src-kernel/plugin-api.h"

/**
 * @brief sort the list with given type
 * 
 * @param type 
 * @param list 
 */
void sort(Type *type, List *list);

#endif // SORT_H
