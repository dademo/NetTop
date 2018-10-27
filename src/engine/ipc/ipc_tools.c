#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "ipc_tools.h"
#include "xml_structures.h"

void _strMallocCpy(
    const char **dst,
    const char *src)
{
    int str_len = 0;

    if (src != NULL)
    {
        str_len = strlen(src);
        *dst = malloc(str_len + 1); /* With '\0' char */
        if (*dst != NULL)
        {
            strcpy((char *)*(dst), src);
        }
        else
        {
            return;
        }
    }
    else
    {
        *dst = NULL;
    }
}

enum SQLITE_DATA_TYPES
_getSQLiteType(
    const char *dataType)
{
    if (dataType != NULL)
    {
        if (strcmp(dataType, SQLITE_INTEGER) == 0)
        {
            return SQLITE_DATA_TYPES_INTEGER;
        }
        if (strcmp(dataType, SQLITE_TEXT) == 0)
        {
            return SQLITE_DATA_TYPES_TEXT;
        }
        if (strcmp(dataType, SQLITE_REAL) == 0)
        {
            return SQLITE_DATA_TYPES_REAL;
        }
        if (strcmp(dataType, SQLITE_BLOB) == 0)
        {
            return SQLITE_DATA_TYPES_BLOB;
        }
        if (strcmp(dataType, SQLITE_NULL) == 0)
        {
            return SQLITE_DATA_TYPES_NULL;
        }
        return SQLITE_DATA_TYPES_UNDEFINED;
    }
    else
    {
        return SQLITE_DATA_TYPES_UNDEFINED;
    }
}

const char *const _getSQLiteTypeString(enum SQLITE_DATA_TYPES dataType)
{
    switch (dataType)
    {
    case SQLITE_DATA_TYPES_INTEGER:
        return "INTEGER";
        break;
    case SQLITE_DATA_TYPES_TEXT:
        return "TEXT";
        break;
    case SQLITE_DATA_TYPES_REAL:
        return "REAL";
        break;
    SQLITE_DATA_TYPES_BLOB:
        return "BLOB";
        break;
    SQLITE_DATA_TYPES_NULL:
        return "NULL";
        break;
    default:
        return "";
        break;
    }
}