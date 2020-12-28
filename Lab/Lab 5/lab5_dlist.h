#ifndef _LAB5_DLIST_H_
#define _LAB5_DLIST_H_

// Data type stored in the dlist
typedef enum dlistValueType_t
{
    DLIST_INT = 0x01,    // list contains int values
    DLIST_STR = 0x02,    // list contains char* values
    DLIST_DOUBLE = 0x03, // list contains double values
    DLIST_UNKOWN = 0x00
} dlistValueType;

// Different options for sorting
// String are sorted in lexical order
typedef enum dlistSortMethod_t
{
    DLIST_SORT_RAND = 0x01, // Randomize entries
    DLIST_SORT_INC = 0x02,  // Sort in ascending order
    DLIST_SORT_DEC = 0x03,  // Sort in descending order
    DLIST_SORT_UNKOWN = 0x00
} dlistSortMethod;

// The value stored in the dlist
// Research online what an "union" is.
typedef union dlistValue_t {
    int intValue;
    double doubleValue;
    char *strValue;
} dlistValue;

typedef void *dlist;
typedef const void *dlist_const;

dlist createDlist(dlistValueType type);
// EFFECTS: allocate and create a new dlist of datatype 'type' object
// returns the created dlist object on success
// returns NULL on error

int dlistIsEmpty(dlist_const this);
// REQUIRES: argument 'this' is non-null and is a valid list
// EFFECTS: return whether the list 'this' is empty

void dlistAppend(dlist this, const char *key, dlistValue value);
// REQUIRES: type instantiated in 'value' corresponds to the datatype in the list
// argument 'this' and 'key' are both non-null and valid lists
// EFFECTS : add a line of form "key=value" to the tail of the dlist
// MODIFIES: modifies 'this'.

void dlistSort(dlist_const this, dlist listDst, dlistSortMethod method);
// REQUIRES: argument 'this' is not null and is a valid list
// argument 'listDst' is not null and is a valid list
// EFFECTS: Sort 'listSrc' using method given by 'method'.
// Put the results in 'listDst' on success
// Leave 'listDst' unchanged on failure
// * Note 'listDst' may be non-empty when invoked.
// * Be very careful with memory management regarding string lists.
// MODIFIES: argument 'listDst'

void dlistPrint(dlist_const this);
// REQUIRES: argument 'this' is not null and is a valid list
// EFFECTS: Print the content of 'this' in the required format to standard output.
// A newline must be followed after each line (including the last one).

void dlistFree(dlist this);
// REQUIRES: argument 'this' is <EITHER> NULL or a valid list
// EFFECTS: if 'this' is NULL do nothing, other wise frees the list
// By freeing the list the user also needs to free the data
// the list manages

#endif
