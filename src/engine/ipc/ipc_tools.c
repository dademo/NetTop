#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "ipc_tools.h"

void
_strMallocCpy(
    const char **dst,
    const char *src)
{
    int str_len = 0;

    if (src != NULL)
    {
        str_len = strlen(src);
        *dst = malloc(str_len + 1); /* With '\0' char */
        strcpy(*dst, src);
    }
    else
    {
        *dst = NULL;
    }
}