/**
 * @author Qinhang Wu (william_wu@sjtu.edu.cn)
 * @brief VE482 l5 ex1
 * @cdate 2020-10-27
 */

#ifndef SORT_H
#define SORT_H

#include "list.h"
#include <assert.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>

#define SORT_RAND 0
#define SORT_INC  1
#define SORT_DEC  2
#define SORT_NULL -1

#define DATA_CHAR   0
#define DATA_INT    1
#define DATA_DOUBLE 2
#define DATA_NULL   -1

typedef struct _type{
    int sortType;
    int dataType;
} Type;

/**
 * @brief sort the list with given type
 * 
 * @param type 
 * @param list 
 */
void sort(Type *type, List *list);

#endif // SORT_H
