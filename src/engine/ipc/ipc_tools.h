#ifndef IPC_TOOLS_H_INCLUDED
#define IPC_TOOLS_H_INCLUDED

#include <libxml/xmlschemas.h>
#include <libxml/xpath.h>

#include "xml_structures.h"

#define _FREE(data)         \
    if (data != NULL)       \
    {                       \
        free((void *)data); \
    }

#define _XMLFREE(data) \
    if (data != NULL)  \
    {                  \
        xmlFree(data); \
    }

#define _STRCMP(s1, s2) (strcmp(s1, s2) == 0)

#define _RES(field, res)                           \
    if (res != 0)                                  \
    {                                              \
        char buff[2048];                           \
        sprintf(buff, "Error at field %s", field); \
        do_log2(buff, LOG_LEVEL_ERROR);            \
        freeAll();                                 \
        return 1;                                  \
    }

#define _SELECT_RES(field, res)                              \
    if (res != 0)                                            \
    {                                                        \
        char buff[2048];                                     \
        sprintf(buff, "Error at field %s", field);           \
        do_log2(buff, LOG_LEVEL_ERROR);                      \
                                                             \
        xmlXPathGetNode_clean(&sub_xpathCtx, &sub_xpathObj); \
        xmlXPathGetNode_clean(&xpathCtx, &xpathObj);         \
        freeAll2();                                          \
        freeAll();                                           \
        return 1;                                            \
    }

void _strMallocCpy(
    char **dst,
    const char *src);

int getVal(
    int *outVal,
    char *str);

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
    char **outBuff,
    char *xpathExpr,
    xmlDocPtr doc,
    int resultRequired,
    char *baseNodeName);

int extractSubDocumentMulti(
    char ***outBuff,
    int *nBuff,
    char *xpathExpr,
    xmlDocPtr doc,
    int resultRequired);

int xmlXPathGetNode(
    xmlXPathContextPtr *xpathCtx, /* A pointer to the xmlXPathContextPtr object we will use */
    xmlXPathObjectPtr *xpathObj,  /* A pointer to the xmlXPathObjectPtr object we will use */
    xmlDocPtr doc,                /* The coument to use */
    char *xpathExpr,              /* The xpath expression to evaluate */
    xmlNodePtr base,              /* Optionnal: The base of research */
    int resultRequired            /* Is the result required */
);

void xmlXPathGetNode_clean(
    xmlXPathContextPtr *xpathCtx,
    xmlXPathObjectPtr *xpathObj);

/**
 * 
 * @Return 1 if multiple nodes found or an error; 0 if OK
  */
int xmlXPathGetAttribute(
    xmlDocPtr doc,      /* The coument to use */
    char *xpathExpr,    /* The xpath expression to evaluate */
    xmlNodePtr base,    /* Optionnal: The base of research */
    int resultRequired, /* Is the result required */
    char *attribute,    /* The attribute to extract */
    char **outBuff      /* The output buffer pointer (auto-alloc) */
);

int xmlXPathGetValue(
    xmlDocPtr doc,      /* The coument to use */
    char *xpathExpr,    /* The xpath expression to evaluate */
    xmlNodePtr base,    /* Optionnal: The base of research */
    int resultRequired, /* Is the result required */
    char **outBuff      /* The output buffer pointer (auto-alloc) */
);

#endif