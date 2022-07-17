#include <stdlib.h>  // for calloc && realloc && free
#include <stdio.h>
#include <stdbool.h>

#include "include/list.h"
#include "libs/colors/colors.h"

bool listOK(list_t *list)
{
    return  list != NULL            && list->nodes != NULL          &&
            list->size >= 0         && list->size <= list->capacity &&
            list->headIdx != POISON && list->freeNodeIdx != POISON;
}

bool indexIsOK(list_t *list, int index)
{
    return index < list->capacity && index >= 0;
}

EXIT_CODES listCtor(list_t *list, int listCapacity)
{
    // Error check
    if (list == NULL)
    {
        PRINT_ERROR_TRACING_MESSAGE(EXIT_CODES::PASSED_OBJECT_IS_NULLPTR);
        return EXIT_CODES::PASSED_OBJECT_IS_NULLPTR;
    }

    if (list->headIdx != POISON || list->freeNodeIdx != POISON ||
        list->size != POISON || list->capacity != POISON)
    {
        PRINT_ERROR_TRACING_MESSAGE(LIST_EXIT_CODES::OLD_LIST_PASSED);
        return EXIT_CODES::BAD_OBJECT_PASSED;
    }

    if (listCapacity < 0)
    {
        PRINT_ERROR_TRACING_MESSAGE(EXIT_CODES::BAD_OBJECT_PASSED);
        return EXIT_CODES::BAD_OBJECT_PASSED;
    }

    // Allocate some nodes
    node_t *nodes = (node_t *) calloc(listCapacity, sizeof(node_t));
    CHECK_CALLOC_RESULT(nodes);

    // Init fields
    list->nodes         = nodes;
    list->capacity      = listCapacity;
    list->size          = 0;
    list->headIdx       = 0;
    list->freeNodeIdx   = 1;

    // Setup head node
    list->nodes[list->headIdx]  = { list->headIdx, POISON, list->headIdx };
    
    // Setup other nodes
    for (int node = 1; node < list->capacity; ++node)
    {
        list->nodes[node].prev = POISON;
        list->nodes[node].data = POISON;
        list->nodes[node].next = node + 1;
    }

    // Setup last node
    list->nodes[list->capacity - 1].next = 0;
    
    return EXIT_CODES::NO_ERRORS;
}

EXIT_CODES listDtor(list_t *list)
{
    // Error check
    OBJECT_VERIFY(list, listOK, listCreateGraph);

    // Destruction
    free(list->nodes);

    // Poison list fields
    list->nodes         = NULL;
    list->headIdx       = POISON;
    list->freeNodeIdx   = POISON;
    list->capacity      = POISON;
    list->size          = POISON;
    
    return EXIT_CODES::NO_ERRORS;
}

EXIT_CODES listGetNextNode(list_t *list, int index, int *result)
{
    // Error check
    OBJECT_VERIFY(list, listOK, listCreateGraph);
    if (!indexIsOK(list, index))
    {
        PRINT_ERROR_TRACING_MESSAGE(LIST_EXIT_CODES::BAD_LIST_INDEX_PASSED);
        return EXIT_CODES::BAD_OBJECT_PASSED;
    }

    *result = list->nodes[index].next;

    return EXIT_CODES::NO_ERRORS;
}

EXIT_CODES listGetPrevNode(list_t *list, int index, int *result)
{
    // Error check
    OBJECT_VERIFY(list, listOK, listCreateGraph);
    if (!indexIsOK(list, index))
    {
        PRINT_ERROR_TRACING_MESSAGE(LIST_EXIT_CODES::BAD_LIST_INDEX_PASSED);
        return EXIT_CODES::BAD_OBJECT_PASSED;
    }

    *result = list->nodes[index].prev;

    return EXIT_CODES::NO_ERRORS;
}

EXIT_CODES listInsertAfter(list_t *list, int index, nodeDataType_t value)
{
    // Error check
    OBJECT_VERIFY(list, listOK, listCreateGraph);
    if (!indexIsOK(list, index))
    {
        PRINT_ERROR_TRACING_MESSAGE(LIST_EXIT_CODES::BAD_LIST_INDEX_PASSED);
        return EXIT_CODES::BAD_OBJECT_PASSED;
    }

    // Check for reallocation
    if (list->size == list->capacity - 1)
    {
        if (listCapacityIncrease(list) != EXIT_CODES::NO_ERRORS)
        {
            PRINT_ERROR_TRACING_MESSAGE(LIST_EXIT_CODES::BAD_LIST_CAPACITY_INCREASE_FUNC_RESULT);
            return EXIT_CODES::BAD_FUNC_RESULT;
        }
    }

    // Get insertion index
    int insertIndex     = list->freeNodeIdx;
    list->freeNodeIdx   = list->nodes[list->freeNodeIdx].next;

    // Connect inserted node with surrounded
    list->nodes[insertIndex].prev   = index;
    list->nodes[insertIndex].data   = value;
    list->nodes[insertIndex].next   = list->nodes[index].next;

    // Connect surrounded nodes with inserted node
    list->nodes[list->nodes[index].next].prev   = insertIndex;
    list->nodes[index].next                     = insertIndex;

    list->size++;

    if (list->freeNodeIdx == 0)
    {
        list->freeNodeIdx = list->size + 1;
    }

    return EXIT_CODES::NO_ERRORS;
}

EXIT_CODES listAppend(list_t *list, nodeDataType_t value)
{
    // Error check
    OBJECT_VERIFY(list, listOK, listCreateGraph);

    // Append
    IS_ERROR(listInsertAfter(list, list->nodes[list->headIdx].prev, value))
    {
        PRINT_ERROR_TRACING_MESSAGE(LIST_EXIT_CODES::BAD_LIST_INSERT_AFTER_RESULT_IN_APPEND);
        return EXIT_CODES::BAD_FUNC_RESULT;
    }

    return EXIT_CODES::NO_ERRORS;
}

EXIT_CODES listRemove(list_t *list, int index)
{
    // Error check
    OBJECT_VERIFY(list, listOK, listCreateGraph);
    if (!indexIsOK(list, index))
    {
        PRINT_ERROR_TRACING_MESSAGE(EXIT_CODES::BAD_OBJECT_PASSED);
        return EXIT_CODES::BAD_OBJECT_PASSED;
    }

    if (index == list->headIdx)
    {
        PRINT_ERROR_TRACING_MESSAGE(LIST_EXIT_CODES::YOU_CANT_REMOVE_HEAD_NODE);
        return EXIT_CODES::BAD_OBJECT_PASSED;
    }

    // Relink surrounding nodes
    list->nodes[list->nodes[index].next].prev = list->nodes[index].prev;
    list->nodes[list->nodes[index].prev].next = list->nodes[index].next;

    // Delete `index` node (poison it)
    list->nodes[index].prev = POISON;
    list->nodes[index].data = POISON;
    list->nodes[index].next = list->freeNodeIdx;
    list->freeNodeIdx       = index;

    list->size--;

    return EXIT_CODES::NO_ERRORS;
}

EXIT_CODES listCapacityIncrease(list_t *list)
{
    // Error check
    OBJECT_VERIFY(list, listOK, listCreateGraph);
    
    // Capacity increase
    int newCapacity = list->capacity * CAPACITY_INCREASE_COEFF;
    node_t *new_nodes = (node_t *) realloc(list->nodes, sizeof(node_t) * newCapacity);
    CHECK_REALLOC_RESULT(new_nodes);

    // Update nodes
    list->nodes = new_nodes;

    // Link new allocated nodes
    for (int node = list->capacity; node < newCapacity; ++node)
    {
        list->nodes[node].prev = POISON;
        list->nodes[node].data = POISON;
        list->nodes[node].next = node + 1;
    }
    list->nodes[newCapacity - 1].next = 0;

    // Update capacity
    list->capacity  = newCapacity;

    return EXIT_CODES::NO_ERRORS;
}

EXIT_CODES listDump(list_t *list)
{
    // Error check
    if (list == NULL)
    {
        PRINT_ERROR_TRACING_MESSAGE(EXIT_CODES::PASSED_OBJECT_IS_NULLPTR);
        return EXIT_CODES::PASSED_OBJECT_IS_NULLPTR;
    }

    PRINT_VAR(list->headIdx,        "%d");
    PRINT_VAR(list->freeNodeIdx,    "%d");
    PRINT_VAR(list->size,           "%d");
    PRINT_VAR(list->capacity,       "%d");

    // Dump
    for (int node = 0; node < list->size + 1; ++node)
    {
        printf(YELLOW "[%d]:\n" RESET, node);
        printf("\tlist->nodes[%d].prevIndx: " YELLOW "%d" RESET " (%d)\n", node, list->nodes[node].prev, list->nodes[list->nodes[node].prev].data);
        printf("\tlist->nodes[%d].data: %d\n", node, list->nodes[node].data);
        printf("\tlist->nodes[%d].nextIndx: " YELLOW "%d" RESET " (%d)\n", node, list->nodes[node].next, list->nodes[list->nodes[node].next].data);
    }
    putchar('\n');

    return EXIT_CODES::NO_ERRORS;
}

EXIT_CODES listCreateGraph(list_t *list)
{
    // Error check
    OBJECT_VERIFY(list, listOK, listCreateGraph);

    // Output file
    FILE *fs = fopen(OUTPUT_DOT_FILENAME, "w");
    CHECK_FOPEN_RESULT(fs);

    // Init head node style
    fprintf(fs, "digraph List\n{\n");
    fprintf(fs, "\tNode0 [shape=\"record\", label=\"<np0> %d | {0 (head) | %d} | <nn0> %d\", style=filled, fillcolor=orange];\n",
                list->nodes[0].prev, list->nodes[0].data, list->nodes[0].next);

    // Init other nodes style
    for (int node = 1; node < list->capacity; ++node)
    {
        fprintf(fs, "\tNode%d [shape=\"record\", label=\"<np%d> %d | {%d | %d} | <nn%d> %d\"];\n",
                    node, node, list->nodes[node].prev, node, list->nodes[node].data, node, list->nodes[node].next);
    }

    // Place nodes
    for (int node = 0; node < list->capacity - 1; ++node)
    {
        fprintf(fs, "\tNode%d -> Node%d[style=invis];\n", node, node + 1);
    }

    // Connect nodes using prev and next
    int node = 0;
    for (; node < list->capacity; ++node)
    {
        fprintf(fs, "\tNode%d:nn%d -> Node%d[color=\"%s\"];\n",
                    node, node, list->nodes[node].next,
                    (list->nodes[node].prev == POISON ? "black" : "chartreuse"));
        
        if (list->nodes[node].prev != POISON)
        {
            fprintf(fs, "\tNode%d:np%d -> Node%d[color=\"red\"];\n",
                        node, node, list->nodes[node].prev);
        }
    }
    // PRINT_VAR(node, "%d");
    // PRINT_VAR(list->freeNodeIdx, "%d");

    // printf("node: %d\n", node);
    // printf("list->freeNodeIdx: %d\n", list->freeNodeIdx);
    if (node != list->freeNodeIdx)
    {
        fprintf(fs, "\tfree[shape=\"record\", style=filled, fillcolor=azure3];\n");
        fprintf(fs, "\tfree -> Node%d;\n", list->freeNodeIdx);
    }
    fprintf(fs, "}\n");

    fclose(fs);

    system(CREATE_GRAPH_COMMAND);

    return EXIT_CODES::NO_ERRORS;
}
