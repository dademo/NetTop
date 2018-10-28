#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <libxml/parser.h>
#include <libxml/tree.h>
#include <libxml/xinclude.h>
#include <libxml/xmlIO.h>
#include <libxml/xmlschemas.h>
#include <libxml/xpath.h>

#include "../tools/log.h"
#include "xml_structures.h"
#include "ipc_tools.h"

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
            fprintf(stderr, "Unable to malloc");
            return;
        }
    }
    else
    {
        printf("src == NULL\n");
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

const char *const
_getSQLiteTypeString(enum SQLITE_DATA_TYPES dataType)
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

int xml_apply_xsd(
    const xmlDocPtr doc,
    const char *rawXsd)
{
    //xmlDocPtr doc = NULL;
    xmlSchemaParserCtxtPtr schemaParserCtxt = NULL;
    xmlSchemaPtr parsedSchema = NULL;
    xmlSchemaValidCtxtPtr validationContext = NULL;
    int validateDoc;

    /* Parsing the input xml */
    /*doc = xmlReadMemory(rawXml, strlen(rawXml), "in.xml", NULL, 0);
    if (doc == NULL)
    {
        fprintf(stderr, "Unable to open XML file\n");
        _xmlDoClean(
            doc,
            schemaParserCtxt,
            parsedSchema,
            validationContext);
        return 1;
    }*/
    // Getting the schema structure
    schemaParserCtxt = xmlSchemaNewMemParserCtxt(rawXsd, strlen(rawXsd));
    if (schemaParserCtxt == NULL)
    {
        do_log2("Unable to open XSD file", LOG_LEVEL_ERROR);
        _xmlDoClean(
            schemaParserCtxt,
            parsedSchema,
            validationContext);
        return 1;
    }
    parsedSchema = xmlSchemaParse(schemaParserCtxt);
    if (parsedSchema == NULL)
    {
        do_log2("Unable to parse XSD file", LOG_LEVEL_ERROR);
        _xmlDoClean(
            schemaParserCtxt,
            parsedSchema,
            validationContext);
        return 1;
    }
    validationContext = xmlSchemaNewValidCtxt(parsedSchema);
    if (validationContext == NULL)
    {
        do_log2("Unable to create validation context with XSD file", LOG_LEVEL_ERROR);
        _xmlDoClean(
            schemaParserCtxt,
            parsedSchema,
            validationContext);
        return 1;
    }
    validateDoc = xmlSchemaValidateDoc(validationContext, doc);
    if (validateDoc != 0)
    {
        _xmlDoClean(
            schemaParserCtxt,
            parsedSchema,
            validationContext);
        if (validateDoc < 0)
        {
            do_log2("libxml2 internal error", LOG_LEVEL_ERROR);
            return 1;
        }
        if (validateDoc > 0)
        {
            do_log2("XML file validation error", LOG_LEVEL_ERROR);
            return 1;
        }
    }

    xmlSchemaFreeParserCtxt(schemaParserCtxt);
    xmlSchemaFree(parsedSchema);
    xmlSchemaFreeValidCtxt(validationContext);
    return 0;
}

void _xmlDoClean(
    xmlSchemaParserCtxtPtr schemaParserCtxt,
    xmlSchemaPtr parsedSchema,
    xmlSchemaValidCtxtPtr validationContext)
{
    if (schemaParserCtxt != NULL)
    {
        xmlSchemaFreeParserCtxt(schemaParserCtxt);
    }
    if (parsedSchema != NULL)
    {
        xmlSchemaFree(parsedSchema);
    }
    if (validationContext != NULL)
    {
        xmlSchemaFreeValidCtxt(validationContext);
    }
}

int extractSubDocument(
    const char **outBuff,
    char *xpathExpr,
    xmlDocPtr doc,
    int resultRequired,
    char *baseNodeName)
{
    xmlXPathContextPtr xpathCtx = NULL;
    xmlXPathObjectPtr xpathObj = NULL;
    xmlDocPtr newDoc = NULL;
    xmlNodePtr newDocBase = NULL;
    unsigned char *xmlBuff = NULL;
    int xmlBuffSize = 0;

    xpathCtx = xmlXPathNewContext(doc);
    if (xpathCtx == NULL)
    {
        do_log2("Unable to create xpathCtx", LOG_LEVEL_ERROR);
        return 0;
    }

    xpathObj = xmlXPathEvalExpression(xpathExpr, xpathCtx);
    if (xpathObj == NULL && resultRequired == 1)
    {
        do_log2("Unable to evaluate xPath expression which is required", LOG_LEVEL_ERROR);
        xmlXPathFreeContext(xpathCtx);
        return 0;
    }

    /* Copying the whole content */
    newDoc = xmlNewDoc("1.0");
    newDocBase = xmlNewNode(NULL, baseNodeName);
    xmlDocSetRootElement(newDoc, newDocBase);
    for (int i = 0; i < xpathObj->nodesetval->nodeNr; i++)
    {
        if (xpathObj->nodesetval->nodeTab[i] != NULL)
        {
            xmlAddChildList(newDocBase, xmlCopyNodeList(xpathObj->nodesetval->nodeTab[i]));
        }
    }
    xmlDocDumpFormatMemory(newDoc, &xmlBuff, &xmlBuffSize, 0);

    *outBuff = malloc(xmlBuffSize * sizeof(unsigned char));

    if (*outBuff == NULL)
    {
        do_log2("Unable to malloc", LOG_LEVEL_ERROR);

        xmlFree(xmlBuff);
        xmlXPathFreeObject(xpathObj);
        xmlXPathFreeContext(xpathCtx);
        return 0;
    }

    _strMallocCpy(outBuff, xmlBuff);

    xmlFree(xmlBuff);
    xmlXPathFreeObject(xpathObj);
    xmlXPathFreeContext(xpathCtx);
    xmlFreeDoc(newDoc);
    return 0;
}

int extractSubDocumentMulti(
    const char ***outBuff,
    int *nBuff,
    char *xpathExpr,
    xmlDocPtr doc,
    int resultRequired)
{
    xmlXPathContextPtr xpathCtx = NULL;
    xmlXPathObjectPtr xpathObj = NULL;
    xmlDocPtr newDoc = NULL;
    xmlNodePtr newDocBase = NULL;
    unsigned char *xmlBuff = NULL;
    int xmlBuffSize = 0;
    int arr_size = 0;
    const char **tmpOutBuff = NULL;

    xpathCtx = xmlXPathNewContext(doc);
    if (xpathCtx == NULL)
    {
        do_log2("Unable to create xpathCtx", LOG_LEVEL_ERROR);
        return 0;
    }

    xpathObj = xmlXPathEvalExpression(xpathExpr, xpathCtx);
    if (xpathObj == NULL && resultRequired == 1)
    {
        do_log2("Unable to evaluate xPath expression which is required", LOG_LEVEL_ERROR);
        xmlXPathFreeContext(xpathCtx);
        return 0;
    }

    /* Copying the whole content */
    for (int i = 0; i < xpathObj->nodesetval->nodeNr; i++)
    {
        if (xpathObj->nodesetval->nodeTab[i] != NULL)
        {
            newDoc = xmlNewDoc("1.0");
            xmlNodePtr rootNote = xmlCopyNode(xpathObj->nodesetval->nodeTab[i], 1);
            xmlDocSetRootElement(newDoc, rootNote);
            xmlDocDumpFormatMemory(newDoc, &xmlBuff, &xmlBuffSize, 0);

            tmpOutBuff = realloc(tmpOutBuff, (arr_size + 1) * sizeof(char **));
            if (tmpOutBuff != NULL)
            {
                _strMallocCpy(tmpOutBuff + arr_size, xmlBuff);
                arr_size++;
            }
            else
            {
                do_log2("Unable to malloc", LOG_LEVEL_ERROR);
                for (int i = 0; i < arr_size; i++)
                {
                    free((char *)(tmpOutBuff)[i]);
                }
                xmlFree(xmlBuff);
                xmlFreeDoc(newDoc);
                xmlXPathFreeObject(xpathObj);
                xmlXPathFreeContext(xpathCtx);
                return 1;
            }
            xmlFree(xmlBuff);
            xmlFreeDoc(newDoc);
        }
    }
    xmlXPathFreeObject(xpathObj);
    xmlXPathFreeContext(xpathCtx);

    *outBuff = tmpOutBuff;
    *nBuff = arr_size;

    return 0;
}