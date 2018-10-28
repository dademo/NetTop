#include <stdlib.h>
#include <stdio.h>
#include <string.h>
/* libxml2 */
#include <libxml/tree.h>
/* defs */
#include "xml_structures.h"
#include "xsd_data.h"
/* tools */
#include "ipc_tools.h"

/* Functions */
/* Master */

struct xml_master_config
mkMasterConfig(
    const char *configName,
    const char *inConfig)
{
    struct xml_master_config config;

    config.actionType = xml_master_config_str;

    _strMallocCpy((char **)&(config.configName), configName);
    _strMallocCpy((char **)&(config.inConfig), inConfig);

    return config;
}

struct xml_master_httpRequestQuery_headers
mkQueryHeaders(
    const char *accept,
    const char *accept_charset,
    const char *accept_language,
    const char *user_agent)
{
    struct xml_master_httpRequestQuery_headers headers;

    _strMallocCpy((char **)&(headers.accept), accept);
    _strMallocCpy((char **)&(headers.accept_charset), accept_charset);
    _strMallocCpy((char **)&(headers.accept_language), accept_language);
    _strMallocCpy((char **)&(headers.user_agent), user_agent);

    return headers;
}

struct xml_master_httpRequestQuery_postArgs
mkQueryPostArgs(
    const char *arg_name,
    const char *arg)
{
    struct xml_master_httpRequestQuery_postArgs postArgs;

    _strMallocCpy((char **)&(postArgs.arg_name), arg_name);
    _strMallocCpy((char **)&(postArgs.arg), arg);

    return postArgs;
}

struct xml_master_httpRequestQuery
mkhttpRequestQuery(
    const char *queryType,
    const char *path,
    const char *accept,
    const char *accept_charset,
    const char *accept_language,
    const char *user_agent)
{
    struct xml_master_httpRequestQuery httpRequestQuery;

    httpRequestQuery.actionType = xml_master_httpRequestQuery_str;

    _strMallocCpy((char **)&(httpRequestQuery.queryType), queryType);
    _strMallocCpy((char **)&(httpRequestQuery.path), path);
    httpRequestQuery.headers = mkQueryHeaders(
        accept,
        accept_charset,
        accept_language,
        user_agent);
    httpRequestQuery.nArgs = 0;
    httpRequestQuery.postArgs = NULL;

    return httpRequestQuery;
}

int mkhttpRequestQuery_addPostArg(
    struct xml_master_httpRequestQuery *httpRequestQuery,
    const char *arg_name,
    const char *arg)
{
    struct xml_master_httpRequestQuery_postArgs postArgs = mkQueryPostArgs(arg_name, arg);
    struct xml_master_httpRequestQuery_postArgs *new_allArgs = realloc(
        httpRequestQuery->postArgs,
        (httpRequestQuery->nArgs + 1) * sizeof(struct xml_master_httpRequestQuery_postArgs));

    if (new_allArgs != NULL)
    {
        /* OK */
        memcpy(
            new_allArgs + httpRequestQuery->nArgs,
            &postArgs,
            sizeof(struct xml_master_httpRequestQuery_postArgs));

        httpRequestQuery->postArgs = new_allArgs;
        httpRequestQuery->nArgs++;
        return 0;
    }
    else
    {
        /* Error, cleaning */
        free_xml_master_httpRequestQuery_postArgs(&postArgs);
        return 1;
    }
}

struct xml_master_sqlQueryError
mkSqlQueryError(
    const char *query,
    unsigned int code,
    const char *error)
{
    struct xml_master_sqlQueryError sqlQueryError;

    sqlQueryError.actionType = xml_master_sqlQueryError_str;

    _strMallocCpy((char **)&sqlQueryError.query, query);
    sqlQueryError.code = code;
    _strMallocCpy((char **)&sqlQueryError.error, error);

    return sqlQueryError;
}

struct xml_master_sqlQueryModifResult
mkSqlQueryModifResult(
    const char *query,
    unsigned int changes)
{
    struct xml_master_sqlQueryModifResult sqlQueryModifResult;

    sqlQueryModifResult.actionType = xml_master_sqlQueryModifResult_str;

    _strMallocCpy((char **)&sqlQueryModifResult.query, query);
    sqlQueryModifResult.changes = changes;

    return sqlQueryModifResult;
}

struct xml_master_sqlQuerySelectResult_rowValue
mkSqlQuerySelectResultRowValue(
    unsigned int columnId,
    enum SQLITE_DATA_TYPES dataType,
    const char *colName,
    const char *data)
{
    struct xml_master_sqlQuerySelectResult_rowValue sqlQuerySelectResultRowValue;

    sqlQuerySelectResultRowValue.columnId = columnId;
    sqlQuerySelectResultRowValue.dataType = dataType;
    _strMallocCpy((char **)&sqlQuerySelectResultRowValue.colName, colName);
    _strMallocCpy((char **)&sqlQuerySelectResultRowValue.data, data);

    return sqlQuerySelectResultRowValue;
}

struct xml_master_sqlQuerySelectResult_row
mkSqlQuerySlectResultRow()
{
    struct xml_master_sqlQuerySelectResult_row sqlQuerySelectResultRow;

    sqlQuerySelectResultRow.nCols = 0;
    sqlQuerySelectResultRow.cols = NULL;

    return sqlQuerySelectResultRow;
}

struct xml_master_sqlQuerySelectResult
mkSqlQuerySelectResult(
    const char *query)
{
    struct xml_master_sqlQuerySelectResult sqlQuerySelectResult;

    sqlQuerySelectResult.actionType = xml_master_sqlQuerySelectResult_str;

    _strMallocCpy((char **)&sqlQuerySelectResult.query, query);
    sqlQuerySelectResult.nRows = 0;
    sqlQuerySelectResult.rows = NULL;

    return sqlQuerySelectResult;
}

int mkSqlQuerySelectResultRowValue_addRowValue(
    struct xml_master_sqlQuerySelectResult_row *sqlQuerySelectResultRow,
    unsigned int columnId,
    const char *dataType,
    const char *colName,
    const char *data)
{
    struct xml_master_sqlQuerySelectResult_rowValue sqlQuerySelectResultRowValue = mkSqlQuerySelectResultRowValue(
        columnId,
        _getSQLiteType(dataType),
        colName,
        data);

    struct xml_master_sqlQuerySelectResult_rowValue *new_cols = realloc(
        sqlQuerySelectResultRow->cols,
        (sqlQuerySelectResultRow->nCols + 1) * sizeof(struct xml_master_sqlQuerySelectResult_rowValue));

    if (new_cols != NULL)
    {
        /* OK */
        memcpy(
            new_cols + sqlQuerySelectResultRow->nCols,
            &sqlQuerySelectResultRowValue,
            sizeof(struct xml_master_sqlQuerySelectResult_rowValue));

        sqlQuerySelectResultRow->cols = new_cols;
        sqlQuerySelectResultRow->nCols++;
        return 0;
    }
    else
    {
        /* Error, cleaning */
        free_xml_master_sqlQuerySelectResult_rowValue(&sqlQuerySelectResultRowValue);
        return 1;
    }
}

int mkSqlQuerySelectResult_addRow(
    struct xml_master_sqlQuerySelectResult *sqlQuerySelectResult,
    struct xml_master_sqlQuerySelectResult_row sqlQuerySelectResultRow)
{
    struct xml_master_sqlQuerySelectResult_row *new_rows = realloc(
        sqlQuerySelectResult->rows,
        (sqlQuerySelectResult->nRows + 1) * sizeof(struct xml_master_sqlQuerySelectResult_row));

    if (new_rows != NULL)
    {
        memcpy(
            new_rows + sqlQuerySelectResult->nRows,
            &sqlQuerySelectResultRow,
            sizeof(struct xml_master_sqlQuerySelectResult_row));

        sqlQuerySelectResult->rows = new_rows;
        sqlQuerySelectResult->nRows++;
        return 0;
    }
    else
    {
        return 1;
    }
}

/* Slave */
struct xml_slave_configQuery
mkConfigQuery()
{
    struct xml_slave_configQuery configQuery;

    configQuery.actionType = xml_slave_configQuery_str;

    configQuery.nConfigQueryName = 0;
    configQuery.allConfigQueryName = NULL;

    return configQuery;
}

int addConfigQuery(
    struct xml_slave_configQuery *configQuery,
    const char *configQueryStr)
{
    char **new_allConfigsQueryName = realloc(
        configQuery->allConfigQueryName,
        (configQuery->nConfigQueryName + 1) * sizeof(char *));
    char *tmpConfigStr = NULL;

    if (new_allConfigsQueryName != NULL)
    {
        // Allocating new space
        _strMallocCpy(&tmpConfigStr, configQueryStr);
        if (tmpConfigStr != NULL)
        {
            new_allConfigsQueryName[configQuery->nConfigQueryName + 1] = tmpConfigStr;

            configQuery->allConfigQueryName = (const char **)new_allConfigsQueryName;
            configQuery->nConfigQueryName++;
            return 0;
        }
        else
        {
            return 1;
        }
    }
    else
    {
        return 1;
    }
}

struct xml_slave_httpRequestAnswer
mkHttpRequestAnswer(
    const char *content_type,
    const char *content_language,
    unsigned int code,
    const char *body)
{
    struct xml_slave_httpRequestAnswer_headers httpRequestAnswerHeader;
    struct xml_slave_httpRequestAnswer httpRequestAnswer;

    httpRequestAnswer.actionType = xml_slave_httpRequestAnswer_str;

    _strMallocCpy((char **)&httpRequestAnswerHeader.content_type, content_type);
    _strMallocCpy((char **)&httpRequestAnswerHeader.content_language, content_language);
    httpRequestAnswer.code = code;
    httpRequestAnswer.headers = httpRequestAnswerHeader;
    _strMallocCpy((char **)&httpRequestAnswer.body, body);

    return httpRequestAnswer;
}

struct xml_slave_log mkLog(
    enum log_level logLevel,
    const char *text)
{
    struct xml_slave_log log;

    log.actionType = xml_slave_log_str;

    log.logLevel = logLevel;
    _strMallocCpy((char **)&log.text, text);

    return log;
}

struct xml_slave_sqlQueryModifQuery
mkSqlQueryModifyQuery(
    const char *query)
{
    struct xml_slave_sqlQueryModifQuery sqlQueryModifQuery;

    sqlQueryModifQuery.actionType = xml_slave_sqlQueryModifQuery_str;

    _strMallocCpy((char **)&sqlQueryModifQuery.query, query);

    return sqlQueryModifQuery;
}

struct xml_slave_sqlQuerySelectQuery
mkSqlQuerySelectQuery(
    const char *query)
{
    struct xml_slave_sqlQuerySelectQuery sqlQuerySelectQuery;

    sqlQuerySelectQuery.actionType = xml_slave_sqlQueryModifQuery_str;

    _strMallocCpy((char **)&sqlQuerySelectQuery.query, query);

    return sqlQuerySelectQuery;
}

/* Generators */
/* Master */
const char *
stringify_xml_master_config(
    struct xml_master_config config)
{
}

const char *
stringify_xml_master_httpRequestQuery(
    struct xml_master_httpRequestQuery httpRequestQuery)
{
}

const char *
stringify_xml_master_sqlQueryError(
    struct xml_master_sqlQueryError sqlQueryError)
{
}

const char *
stringify_xml_master_sqlQueryModifResult(
    struct xml_master_sqlQueryModifResult sqlQueryModifResult)
{
}

const char *
stringify_xml_master_sqlQuerySelectResult(
    struct xml_master_sqlQuerySelectResult sqlQuerySelectResult)
{
}

const char *
stringify_xml_slave_configQuery(
    struct xml_slave_configQuery configQuery)
{
}

const char *
stringify_xml_slave_httpRequestAnswer(
    struct xml_slave_httpRequestAnswer httpRequestAnswer)
{
}

const char *
stringify_xml_slave_log(
    struct xml_slave_log log)
{
}

const char *
stringify_xml_slave_sqlQueryModifQuery(
    struct xml_slave_sqlQueryModifQuery sqlQueryModifQuery)
{
}

const char *
stringify_xml_slave_sqlQuerySelectQuery(
    struct xml_slave_sqlQuerySelectQuery sqlQuerySelectQuery)
{
}

/* Parsers */
/* Master */
int parse_xml_master_config(
    struct xml_master_config **config,
    int *nElems,
    const char *const rawXml)
{
    char **tmpBuff = NULL;
    int res = 0;
    int tabLen = 0;

    xmlDocPtr doc = xmlReadMemory(rawXml, strlen(rawXml), "in_parse_xml_master_config.xml", NULL, 0);
    if (doc == NULL)
    {
        do_log2("Unable to open document", LOG_LEVEL_ERROR);
        return 1;
    }

    if (xml_apply_xsd(doc, globalData_master_config_xsd) != 0)
    {
        do_log2("XSD parsing failed", LOG_LEVEL_ERROR);
        xmlFreeDoc(doc);
        return 1;
    }

    /* Parsing the file */
    //res = extractSubDocument(&tmpBuff, "/message/action/*", doc, 1, "config");
    res = extractSubDocumentMulti(&tmpBuff, &tabLen, "/message/action/*", doc, 1);
    if (res == 0)
    {
        *config = malloc(tabLen * (sizeof(struct xml_master_config)));
        for (int i = 0; i < tabLen; i++)
        {
            xmlDocPtr tmpDoc = xmlReadMemory(tmpBuff[i], strlen(tmpBuff[i]), "in2.xml", NULL, 0);
            xmlNodePtr rootNode = xmlDocGetRootElement(tmpDoc);

            _strMallocCpy((char **)&((*config)[i].configName), rootNode->name);
            (*config)[i].inConfig = tmpBuff[i];

            xmlFreeDoc(tmpDoc);
        }
    }

    *nElems = tabLen;
    xmlFreeDoc(doc);
    return 0;
}

int parse_xml_master_httpRequestQuery(
    struct xml_master_httpRequestQuery *httpRequestQuery,
    const char *const rawXml)
{
    xmlDocPtr doc = xmlReadMemory(rawXml, strlen(rawXml), "in_parse_xml_master_httpRequestQuery.xml", NULL, 0);
    xmlXPathContextPtr xpathCtx;
    xmlXPathObjectPtr xpathObj;
    /* New attributes */
    char *queryType = NULL;
    char *path = NULL;
    char *headers_accept = NULL;
    char *headers_acceptCharset = NULL;
    char *headers_acceptLanguage = NULL;
    char *headers_userAgent = NULL;
    /* Tmp attributes -> POST */
    char *postArgs_name = NULL;
    char *post_args_value = NULL;
    unsigned char *xmlBuff = NULL;
    unsigned char *xmlBuff2 = NULL;
    int res = 0;

    /* Free function */
    void freeAll()
    {
        _FREE(queryType);
        _FREE(path);
        _FREE(headers_accept);
        _FREE(headers_acceptCharset);
        _FREE(headers_acceptLanguage);
        _FREE(headers_userAgent);
        xmlFreeDoc(doc);
    }

    if (doc == NULL)
    {
        do_log2("Unable to open document", LOG_LEVEL_ERROR);
        return 1;
    }

    if (xml_apply_xsd(doc, globalData_master_httpRequest_query_xsd) != 0)
    {
        do_log2("XSD parsing failed", LOG_LEVEL_ERROR);
        xmlFreeDoc(doc);
        return 1;
    }

    /* Parsing the file */
    /* message.action.query */
    _RES("queryType", xmlXPathGetAttribute(doc, "/message/action/query", NULL, 1, "type", &queryType));
    _RES("path", xmlXPathGetValue(doc, "/message/action/query/path", NULL, 1, &path));
    _RES("headers_accept", xmlXPathGetValue(doc, "/message/action/query/headers/Accept", NULL, 1, &headers_accept));
    _RES("headers_acceptCharset", xmlXPathGetValue(doc, "/message/action/query/headers/Accept-Charset", NULL, 1, &headers_acceptCharset));
    _RES("headers_acceptLanguage", xmlXPathGetValue(doc, "/message/action/query/headers/Accept-Language", NULL, 1, &headers_acceptLanguage));
    _RES("headers_userAgent", xmlXPathGetValue(doc, "/message/action/query/headers/User-Agent", NULL, 1, &headers_userAgent));

    /* Creating */
    *httpRequestQuery = mkhttpRequestQuery(
        queryType,
        path,
        headers_accept,
        headers_acceptCharset,
        headers_acceptLanguage,
        headers_userAgent);

    if (_STRCMP("POST", queryType))
    {
        /* Processing POST values */
        res = xmlXPathGetNode(&xpathCtx, &xpathObj, doc, "/message/action/query/post-args/*", NULL, 0);
        if (res != 0)
        {
            do_log2("Unable to get node", LOG_LEVEL_ERROR);
            freeAll();
            return res;
        }

        for (int i = 0; i < xpathObj->nodesetval->nodeNr; i++)
        {
            xmlBuff = xmlGetProp(xpathObj->nodesetval->nodeTab[i], "name");
            xmlBuff2 = xmlNodeListGetString(doc, xpathObj->nodesetval->nodeTab[i]->children, 1);

            if (mkhttpRequestQuery_addPostArg(httpRequestQuery, xmlBuff, xmlBuff2) != 0)
            {
                do_log2("Unable to add post arg", LOG_LEVEL_ERROR);
                xmlXPathGetNode_clean(&xpathCtx, &xpathObj);

                _XMLFREE(xmlBuff);
                _XMLFREE(xmlBuff2);

                freeAll();
                free_xml_master_httpRequestQuery(httpRequestQuery);
                return 1;
            }
            _XMLFREE(xmlBuff);
            _XMLFREE(xmlBuff2);
        }
        xmlXPathGetNode_clean(&xpathCtx, &xpathObj);
    }

    freeAll();
    return 0;
}

int parse_xml_master_sqlQueryError(
    struct xml_master_sqlQueryError *sqlQueryError,
    const char *const rawXml)
{
    xmlDocPtr doc = xmlReadMemory(rawXml, strlen(rawXml), "in_parse_xml_master_sqlQueryError.xml", NULL, 0);
    /* New attributes */
    char *query = NULL;
    int error_code;
    char *errorStr = NULL;
    /* Working attributes */
    char *buff = NULL;

    /* Free function */
    void freeAll()
    {
        _FREE(query);
        _FREE(errorStr);
        _FREE(buff);
        xmlFreeDoc(doc);
    }

    if (doc == NULL)
    {
        do_log2("Unable to open document", LOG_LEVEL_ERROR);
        return 1;
    }

    if (xml_apply_xsd(doc, globalData_master_sqlquery_error_xsd) != 0)
    {
        do_log2("XSD parsing failed", LOG_LEVEL_ERROR);
        xmlFreeDoc(doc);
        return 1;
    }

    /* Parsing the file */
    _RES("query", xmlXPathGetValue(doc, "/message/action/query", NULL, 1, &query));
    _RES("errorCode", xmlXPathGetAttribute(doc, "/message/action/error", NULL, 1, "code", &buff));
    _RES("error", xmlXPathGetValue(doc, "/message/action/error", NULL, 1, &errorStr));

    /* Converting the int value */
    if (getVal(&error_code, buff) != 0)
        ;
    {
        do_log("Unable to parse error number", LOG_LEVEL_ERROR);
        freeAll();
        return 1;
    }

    *sqlQueryError = mkSqlQueryError(
        query,
        error_code,
        errorStr);

    freeAll();
    return 0;
}

int parse_xml_master_sqlQueryModifResult(
    struct xml_master_sqlQueryModifResult *sqlQueryModifResult,
    const char *const rawXml)
{
    xmlDocPtr doc = xmlReadMemory(rawXml, strlen(rawXml), "in_parse_xml_master_sqlQueryModifResult.xml", NULL, 0);
    /* New attributes */
    char *query = NULL;
    int changes;
    /* Working attributes */
    char *buff = NULL;

    /* Free function */
    void freeAll()
    {
        _FREE(query);
        _FREE(buff);
        xmlFreeDoc(doc);
    }

    if (doc == NULL)
    {
        do_log2("Unable to open document", LOG_LEVEL_ERROR);
        return 1;
    }

    if (xml_apply_xsd(doc, globalData_master_sqlquery_modif_result_xsd) != 0)
    {
        do_log2("XSD parsing failed", LOG_LEVEL_ERROR);
        xmlFreeDoc(doc);
        return 1;
    }

    /* Parsing the file */
    _RES("query", xmlXPathGetValue(doc, "/message/action/query", NULL, 1, &query));
    _RES("changes", xmlXPathGetValue(doc, "/message/action/answer/changes", NULL, 1, &buff));

    /* Converting the int value */
    if (getVal(&changes, buff) != 0)
    {
        do_log("Unable to parse changes number", LOG_LEVEL_ERROR);
        freeAll();
        return 1;
    }

    *sqlQueryModifResult = mkSqlQueryModifResult(
        query,
        changes);

    freeAll();
    return 0;
}

int parse_xml_master_sqlQuerySelectResult(
    struct xml_master_sqlQuerySelectResult *sqlQuerySelectResult,
    const char *const rawXml)
{
    xmlDocPtr doc = xmlReadMemory(rawXml, strlen(rawXml), "in_parse_xml_master_sqlQuerySelectResult.xml", NULL, 0);
    xmlXPathContextPtr xpathCtx;
    xmlXPathObjectPtr xpathObj;
    xmlXPathContextPtr sub_xpathCtx;
    xmlXPathObjectPtr sub_xpathObj;
    /* New attributes */
    char *query = NULL;
    int changes;
    unsigned char *dataColumnIdBuff = NULL;
    int dataColumnId = 0;
    unsigned char *dataTypeBuff = NULL;
    unsigned char *dataColNameBuff = NULL;
    unsigned char *dataBuff = NULL;
    /* Working attributes */
    int res = 0;

    /* Free function */
    void freeAll()
    {
        _FREE(query);

        xmlFreeDoc(doc);
    }

    void freeAll2()
    {
        _XMLFREE(dataColumnIdBuff);
        _XMLFREE(dataTypeBuff);
        _XMLFREE(dataColNameBuff);
        _XMLFREE(dataBuff);

        dataColumnIdBuff = NULL;
        dataTypeBuff = NULL;
        dataColNameBuff = NULL;
        dataBuff = NULL;
    }

    if (doc == NULL)
    {
        do_log2("Unable to open document", LOG_LEVEL_ERROR);
        return 1;
    }

    if (xml_apply_xsd(doc, globalData_master_sqlquery_select_result_xsd) != 0)
    {
        do_log2("XSD parsing failed", LOG_LEVEL_ERROR);
        xmlFreeDoc(doc);
        return 1;
    }

    /* Parsing the file */
    _RES("query", xmlXPathGetValue(doc, "/message/action/query", NULL, 1, &query));

    *sqlQuerySelectResult = mkSqlQuerySelectResult(
        query);

    res = xmlXPathGetNode(&xpathCtx, &xpathObj, doc, "/message/action/answer/*", NULL, 0);
    if (res != 0)
    {
        do_log2("Unable to get node", LOG_LEVEL_ERROR);
        freeAll();
        return res;
    }

    /* For each row */
    for (int i = 0; i < xpathObj->nodesetval->nodeNr; i++)
    {
        struct xml_master_sqlQuerySelectResult_row sqlQuerySelectResult_row = mkSqlQuerySlectResultRow();
        res = xmlXPathGetNode(&sub_xpathCtx, &sub_xpathObj, doc, "./*", xpathObj->nodesetval->nodeTab[i], 0);

        if (res != 0)
        {
            do_log2("Unable to get node", LOG_LEVEL_ERROR);
            freeAll2();
            freeAll();
            return res;
        }

        /* For each column */
        for (int j = 0; j < sub_xpathObj->nodesetval->nodeNr; j++)
        {
            dataColumnIdBuff = xmlGetProp(sub_xpathObj->nodesetval->nodeTab[j], "columnId");
            dataTypeBuff = xmlGetProp(sub_xpathObj->nodesetval->nodeTab[j], "type");
            dataColNameBuff = xmlGetProp(sub_xpathObj->nodesetval->nodeTab[j], "name");
            dataBuff = xmlNodeListGetString(doc, sub_xpathObj->nodesetval->nodeTab[j]->children, 1);

            /* Converting the int value */
            if (getVal(&dataColumnId, dataColumnIdBuff) != 0)
            {
                do_log("Unable to parse columnId number", LOG_LEVEL_ERROR);
                free_xml_master_sqlQuerySelectResult_row(&sqlQuerySelectResult_row);
                free_xml_master_sqlQuerySelectResult(sqlQuerySelectResult);
                xmlXPathGetNode_clean(&sub_xpathCtx, &sub_xpathObj);
                xmlXPathGetNode_clean(&xpathCtx, &xpathObj);
                freeAll2();
                freeAll();
                return 1;
            }

            if (mkSqlQuerySelectResultRowValue_addRowValue(
                    &sqlQuerySelectResult_row,
                    dataColumnId,
                    dataTypeBuff,
                    dataColNameBuff,
                    dataBuff) != 0)
            {
                do_log("Unable to parse columnId number", LOG_LEVEL_ERROR);
                free_xml_master_sqlQuerySelectResult_row(&sqlQuerySelectResult_row);
                free_xml_master_sqlQuerySelectResult(sqlQuerySelectResult);
                xmlXPathGetNode_clean(&sub_xpathCtx, &sub_xpathObj);
                xmlXPathGetNode_clean(&xpathCtx, &xpathObj);
                freeAll2();
                freeAll();
                return 1;
            }
            freeAll2();
        }

        if (mkSqlQuerySelectResult_addRow(
                sqlQuerySelectResult,
                sqlQuerySelectResult_row) != 0)
        {
            do_log2("Unable to add row", LOG_LEVEL_ERROR);
            free_xml_master_sqlQuerySelectResult_row(&sqlQuerySelectResult_row);
            free_xml_master_sqlQuerySelectResult(sqlQuerySelectResult);
        }

        xmlXPathGetNode_clean(&sub_xpathCtx, &sub_xpathObj);
    }
    xmlXPathGetNode_clean(&xpathCtx, &xpathObj);
    
    freeAll();
    return 0;
}

/* Slave */
int parse_xml_slave_configQuery(
    struct xml_slave_configQuery *configQuery,
    const char *const rawXml)
{
    xmlDocPtr doc = xmlReadMemory(rawXml, strlen(rawXml), "in.xml", NULL, 0);
    if (doc == NULL)
    {
        do_log2("Unable to open document", LOG_LEVEL_ERROR);
        return 1;
    }

    if (xml_apply_xsd(doc, globalData_slave_config_query_xsd) != 0)
    {
        do_log2("parse_xml_slave_configQuery: XSD parsing failed", LOG_LEVEL_ERROR);
        xmlFreeDoc(doc);
        return 1;
    }

    /* Parsing the file */

    xmlFreeDoc(doc);
}

int parse_xml_slave_httpRequestAnswer(
    struct xml_slave_httpRequestAnswer *httpRequestAnswer,
    const char *const rawXml)
{
    xmlDocPtr doc = xmlReadMemory(rawXml, strlen(rawXml), "in.xml", NULL, 0);
    if (doc == NULL)
    {
        do_log2("Unable to open document", LOG_LEVEL_ERROR);
        return 1;
    }

    if (xml_apply_xsd(doc, globalData_slave_httpRequest_answer_xsd) != 0)
    {
        do_log2("parse_xml_slave_httpRequestAnswer: XSD parsing failed", LOG_LEVEL_ERROR);
        xmlFreeDoc(doc);
        return 1;
    }

    /* Parsing the file */

    xmlFreeDoc(doc);
}

int parse_xml_slave_log(
    struct xml_slave_log *log,
    const char *const rawXml)
{
    xmlDocPtr doc = xmlReadMemory(rawXml, strlen(rawXml), "in.xml", NULL, 0);
    if (doc == NULL)
    {
        do_log2("Unable to open document", LOG_LEVEL_ERROR);
        return 1;
    }

    if (xml_apply_xsd(doc, globalData_slave_log_xsd) != 0)
    {
        do_log2("parse_xml_slave_log: XSD parsing failed", LOG_LEVEL_ERROR);
        xmlFreeDoc(doc);
        return 1;
    }

    /* Parsing the file */

    xmlFreeDoc(doc);
}

int parse_xml_slave_sqlQueryModifQuery(
    struct xml_slave_sqlQueryModifQuery *sqlQueryModifQuery,
    const char *const rawXml)
{
    xmlDocPtr doc = xmlReadMemory(rawXml, strlen(rawXml), "in.xml", NULL, 0);
    if (doc == NULL)
    {
        do_log2("Unable to open document", LOG_LEVEL_ERROR);
        return 1;
    }

    if (xml_apply_xsd(doc, globalData_slave_sqlquery_modif_query_xsd) != 0)
    {
        do_log2("parse_xml_slave_sqlQueryModifQuery: XSD parsing failed", LOG_LEVEL_ERROR);
        xmlFreeDoc(doc);
        return 1;
    }

    /* Parsing the file */

    xmlFreeDoc(doc);
}

int parse_xml_slave_sqlQuerySelectQuery(
    struct xml_slave_sqlQuerySelectQuery *sqlQuerySelectQuery,
    const char *const rawXml)
{
    xmlDocPtr doc = xmlReadMemory(rawXml, strlen(rawXml), "in.xml", NULL, 0);
    if (doc == NULL)
    {
        do_log2("Unable to open document", LOG_LEVEL_ERROR);
        return 1;
    }

    if (xml_apply_xsd(doc, globalData_slave_sqlquery_select_query_xsd) != 0)
    {
        do_log2("parse_xml_slave_sqlQuerySelectQuery: XSD parsing failed", LOG_LEVEL_ERROR);
        xmlFreeDoc(doc);
        return 1;
    }

    /* Parsing the file */

    xmlFreeDoc(doc);
}

/* Free */
/* Master */
/* xml_master_config */
void free_xml_master_config(
    struct xml_master_config *config)
{
    _FREE(config->configName);
    _FREE(config->inConfig);
}

/* xml_master_httpRequestQuery */
void free_xml_master_httpRequestQuery_headers(
    struct xml_master_httpRequestQuery_headers *httpRequestQuery_headers)
{
    _FREE(httpRequestQuery_headers->accept);
    _FREE(httpRequestQuery_headers->accept_charset);
    _FREE(httpRequestQuery_headers->accept_language);
    _FREE(httpRequestQuery_headers->user_agent);
}

void free_xml_master_httpRequestQuery_postArgs(
    struct xml_master_httpRequestQuery_postArgs *httpRequestQuery_postArgs)
{
    _FREE(httpRequestQuery_postArgs->arg_name);
    _FREE(httpRequestQuery_postArgs->arg);
}

void free_xml_master_httpRequestQuery(
    struct xml_master_httpRequestQuery *httpRequestQuery)
{
    _FREE(httpRequestQuery->queryType);
    _FREE(httpRequestQuery->path);
    free_xml_master_httpRequestQuery_headers(&httpRequestQuery->headers);
    for (int i = 0; i < httpRequestQuery->nArgs; i++)
    {
        free_xml_master_httpRequestQuery_postArgs(httpRequestQuery->postArgs + i);
    }
    _FREE(httpRequestQuery->postArgs);
}

/* xml_master_sqlQueryError */
void free_xml_master_sqlQueryError(
    struct xml_master_sqlQueryError *sqlQueryError)
{
    _FREE(sqlQueryError->query);
    _FREE(sqlQueryError->error);
}

/* xml_master_sqlQueryModifResult */
void free_xml_master_sqlQueryModifResult(
    struct xml_master_sqlQueryModifResult *sqlQueryModifResult)
{
    _FREE(sqlQueryModifResult->query);
}

/* xml_master_sqlQuerySelectResult */
void free_xml_master_sqlQuerySelectResult_rowValue(
    struct xml_master_sqlQuerySelectResult_rowValue *sqlQuerySelectResult_rowValue)
{
    _FREE(sqlQuerySelectResult_rowValue->colName);
    _FREE(sqlQuerySelectResult_rowValue->data);
}

void free_xml_master_sqlQuerySelectResult_row(
    struct xml_master_sqlQuerySelectResult_row *sqlQuerySelectResult_row)
{
    for (int i = 0; i < sqlQuerySelectResult_row->nCols; i++)
    {
        free_xml_master_sqlQuerySelectResult_rowValue(sqlQuerySelectResult_row->cols + i);
    }
    _FREE(sqlQuerySelectResult_row->cols);
}

void free_xml_master_sqlQuerySelectResult(
    struct xml_master_sqlQuerySelectResult *sqlQuerySelectResult)
{
    _FREE(sqlQuerySelectResult->query);
    for (int i = 0; i < sqlQuerySelectResult->nRows; i++)
    {
        free_xml_master_sqlQuerySelectResult_row(sqlQuerySelectResult->rows + i);
    }
    _FREE(sqlQuerySelectResult->rows);
}

/* Slave */
/* xml_slave_configQuery */
void free_xml_slave_configQuery(struct xml_slave_configQuery *configQuery)
{
    for (int i = 0; i < configQuery->nConfigQueryName; i++)
    {
        _FREE(configQuery->allConfigQueryName[i]);
    }
    _FREE(configQuery->allConfigQueryName);
}

/* xml_slave_httpRequestAnswer */
void free_xml_slave_httpRequestAnswer_headers(struct xml_slave_httpRequestAnswer_headers *httpRequestAndwer_headers)
{
    _FREE(httpRequestAndwer_headers->content_type);
    _FREE(httpRequestAndwer_headers->content_language);
}

void free_xml_slave_httpRequestAnswer(struct xml_slave_httpRequestAnswer *httpRequestAnswer)
{
    free_xml_slave_httpRequestAnswer_headers(&(httpRequestAnswer->headers));
    _FREE(httpRequestAnswer->body);
}

/* xml_slave_log */
void free_xml_slave_log(struct xml_slave_log *log)
{
    _FREE(log->text);
}

/* xml_slave_sqlQueryModifQuery */
void free_xml_slave_sqlQueryModifQuery(struct xml_slave_sqlQueryModifQuery *sqlQueryModifQuery)
{
    _FREE(sqlQueryModifQuery->query);
}

/* xml_slave_sqlQuerySelectQuery */
void free_xml_slave_sqlQuerySelectQuery(struct xml_slave_sqlQuerySelectQuery *sqlQuerySelectQuery)
{
    _FREE(sqlQuerySelectQuery->query);
}
