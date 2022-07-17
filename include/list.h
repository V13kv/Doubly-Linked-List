#ifndef LIST_H
#define LIST_H


#define DEBUG_LEVEL 1
#include "libs/debug/debug.h"
#include "settings.h"

enum class LIST_EXIT_CODES
{
    OLD_LIST_PASSED,
    BAD_LIST_CAPACITY_INCREASE_FUNC_RESULT,
    BAD_LIST_INDEX_PASSED,
    BAD_LIST_INSERT_AFTER_RESULT_IN_APPEND,
    YOU_CANT_REMOVE_HEAD_NODE,
};

struct node_t
{
    int prev            = POISON;
    nodeDataType_t data = POISON;
    int next            = POISON;
};

struct list_t
{
    node_t *nodes   = NULL;
    int headIdx     = POISON;
    int freeNodeIdx = POISON;
    
    int size     = POISON;
    int capacity = POISON;
};

bool listOK(list_t *list);
bool indexIsOK(list_t *list, int index);

EXIT_CODES listCtor(list_t *list, int listCapacity = MIN_LIST_NODES);
EXIT_CODES listDtor(list_t *list);
EXIT_CODES listCapacityIncrease(list_t *list);

EXIT_CODES listGetNextNode(list_t *list, int index, int *result);
EXIT_CODES listGetPrevNode(list_t *list, int index, int *result);

EXIT_CODES listInsertAfter(list_t *list, int index, nodeDataType_t value);
EXIT_CODES listAppend(list_t *list, nodeDataType_t value);
EXIT_CODES listRemove(list_t *list, int index);

EXIT_CODES listDump(list_t *list);
EXIT_CODES listCreateGraph(list_t *list);



#endif  // LIST_H
