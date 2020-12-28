/**
 * @author Qinhang Wu (william_wu@sjtu.edu.cn)
 * @brief plugin manager
 * @cdate 2020-11-08
 */

#ifndef PLUGIN_H
#define PLUGIN_H

#include "list.h"

#define SORT_RAND 0
#define SORT_INC  1
#define SORT_DEC  2
#define SORT_NULL -1

#define DATA_CHAR   0
#define DATA_INT    1
#define DATA_DOUBLE 2
#define DATA_NULL   -1

/**
 * @brief a structure containing sort and type data type 
 */
typedef struct _type{
    int sortType;
    int dataType;
} Type;

/**
 * @brief a handle containing plugin utilities
 */
typedef struct _Handle{
  void *handle; // handle for dynamic loading functions
  char type[36]; // name of the file type
  // void (*sort)(Type*, List*); // function pointer for sort
  void (*ui_cmd)(char**); // function pointer for ui_cmd
  // ...
} Handle;

/**
 * @brief try to open and extract all possible plugins
 * 
 * @param handle dynamically allocated set of plugins
 * @return int number of plugins found
 */
int pluginInit(Handle **handle);

#endif // PLUGIN_H
