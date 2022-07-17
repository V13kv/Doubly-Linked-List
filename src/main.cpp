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

    for (int i = 0; i < 2; ++i)
    {
        listInsertAfter(&list, i, i + 1);
    }

    listRemove(&list, 2);
    listInsertAfter(&list, 1, 555);
    listAppend(&list, 888);
    listAppend(&list, 777);
    listAppend(&list, 999);
    listAppend(&list, 54321);
    listCreateGraph(&list);

    IS_ERROR(listDtor(&list))
    {
        EXIT_WITH_TRACE(EXIT_CODES::DESTRUCTOR_ERROR, EXIT_FAILURE);
    }

    return 0;
}
