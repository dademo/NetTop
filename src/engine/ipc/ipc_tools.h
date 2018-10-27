#ifndef IPC_TOOLS_H_INCLUDED
#define IPC_TOOLS_H_INCLUDED

#include "xml_structures.h"

#define _FREE(data) if(data != NULL) { free((void*) data); }

void
_strMallocCpy(
    const char** dst,
    const char* src);

enum SQLITE_DATA_TYPES
_getSQLiteType(
    const char* dataType);

#endif