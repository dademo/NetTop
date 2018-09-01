#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "tools.h"
#include "../config.h"

void* memrchr (const void *block, char c, size_t size)
{
    for(int i = (size-1); i >= 0; i--)
    {
        if(*((char*) block + i) == c)
        {
            return (void*) (block + i);
        }
    }
}

char* getDirName(char* fullPath, size_t size)
{
    static char buffer[DEFAULT_BUFFER_SIZE];
    char* spePos = memrchr(fullPath, '/', size);

    if(spePos == NULL)
    {
        return NULL;
    }

    int length = spePos - fullPath;

    if (length > (DEFAULT_BUFFER_SIZE - 1))
    {
        length = DEFAULT_BUFFER_SIZE - 1;
    }

    memcpy(buffer, fullPath, length);
    buffer[length] = '\0';

    return buffer;
}

char* getFileName(char* fullPath, size_t size)
{
    static char buffer[DEFAULT_BUFFER_SIZE];
    char* spePos = memrchr(fullPath, '/', size);

    if(spePos == NULL)
    {
        return NULL;
    }

    int length = size - (spePos - fullPath);

    if (length > (DEFAULT_BUFFER_SIZE - 1))
    {
        length = DEFAULT_BUFFER_SIZE - 1;
    }

    memcpy(buffer, ++spePos, length);
    buffer[size - length] = '\0';

    return buffer;
}