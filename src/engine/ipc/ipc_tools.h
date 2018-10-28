#ifndef IPC_TOOLS_H_INCLUDED
#define IPC_TOOLS_H_INCLUDED

#include <libxml/xmlschemas.h>
#include "xml_structures.h"

#define _FREE(data)         \
    if (data != NULL)       \
    {                       \
        free((void *)data); \
    }

void _strMallocCpy(
    const char **dst,
    const char *src);

enum SQLITE_DATA_TYPES
_getSQLiteType(
    const char *dataType);

int xml_apply_xsd(
    const xmlDocPtr doc,
    const char *rawXsd);

void _xmlDoClean(
    xmlSchemaParserCtxtPtr schemaParserCtxt,
    xmlSchemaPtr parsedSchema,
    xmlSchemaValidCtxtPtr validationContext);

int extractSubDocument(
    const char **outBuff,
    char *xpathExpr,
    xmlDocPtr doc,
    int resultRequired,
    char *baseNodeName);

int extractSubDocumentMulti(
    const char ***outBuff,
    int *nBuff,
    char *xpathExpr,
    xmlDocPtr doc,
    int resultRequired);

#endif