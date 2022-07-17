#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

#include "include/list.h"

int main()
{
    srand((unsigned int) time(NULL));

    list_t list = {};
    IS_ERROR(listCtor(&list, 4))
    {
        EXIT_WITH_TRACE(EXIT_CODES::CONSTRUCTOR_ERROR, EXIT_FAILURE);
    }

    for (int i = 0; i < 4; ++i)
    {
        listInsertAfter(&list, i, i + 1);
    }

    listRemove(&list, 2);
    listInsertAfter(&list, 4, 555);
    listAppend(&list, 888);
    listAppend(&list, 777);

    int idx = 0;
    listGetNextNode(&list, 4, &idx);
    printf("%d\n", idx);

    listGetPrevNode(&list, 5, &idx);
    printf("%d\n", idx); 

    IS_ERROR(listDtor(&list))
    {
        EXIT_WITH_TRACE(EXIT_CODES::DESTRUCTOR_ERROR, EXIT_FAILURE);
    }

    return 0;
}
