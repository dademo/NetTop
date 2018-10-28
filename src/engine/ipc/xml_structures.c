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

    *((const char **)&(config.actionType)) = xml_master_config_str;

    _strMallocCpy(&(config.inConfig), inConfig);

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

    _strMallocCpy(&(headers.accept), accept);
    _strMallocCpy(&(headers.accept_charset), accept_charset);
    _strMallocCpy(&(headers.accept_language), accept_language);
    _strMallocCpy(&(headers.user_agent), user_agent);

    return headers;
}

struct xml_master_httpRequestQuery_postArgs
mkQueryPostArgs(
    const char *arg_name,
    const char *arg)
{
    struct xml_master_httpRequestQuery_postArgs postArgs;

    _strMallocCpy(&(postArgs.arg_name), arg_name);
    _strMallocCpy(&(postArgs.arg), arg);

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

    *((const char **)&(httpRequestQuery.actionType)) = xml_master_httpRequestQuery_str;

    _strMallocCpy(&(httpRequestQuery.queryType), queryType);
    _strMallocCpy(&(httpRequestQuery.path), path);
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

    *((const char **)&(sqlQueryError.actionType)) = xml_master_sqlQueryError_str;

    _strMallocCpy(&sqlQueryError.query, query);
    sqlQueryError.code = code;
    _strMallocCpy(&sqlQueryError.error, error);

    return sqlQueryError;
}

struct xml_master_sqlQueryModifResult
mkSqlQueryModifResult(
    const char *query,
    unsigned int changes)
{
    struct xml_master_sqlQueryModifResult sqlQueryModifResult;

    *((const char **)&(sqlQueryModifResult.actionType)) = xml_master_sqlQueryModifResult_str;

    _strMallocCpy(&sqlQueryModifResult.query, query);
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
    _strMallocCpy(&sqlQuerySelectResultRowValue.colName, colName);
    _strMallocCpy(&sqlQuerySelectResultRowValue.data, data);

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

    *((const char **)&(sqlQuerySelectResult.actionType)) = xml_master_sqlQuerySelectResult_str;

    _strMallocCpy(&sqlQuerySelectResult.query, query);
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

    *((const char **)&(configQuery.actionType)) = xml_slave_configQuery_str;

    configQuery.nConfigQueryName = 0;
    configQuery.allConfigQueryName = NULL;

    return configQuery;
}

int addConfigQuery(
    struct xml_slave_configQuery *configQuery,
    const char *configQueryStr)
{
    const char **new_allConfigsQueryName = realloc(
        configQuery->allConfigQueryName,
        (configQuery->nConfigQueryName + 1) * sizeof(char *));
    const char *tmpConfigStr = NULL;

    if (new_allConfigsQueryName != NULL)
    {
        // Allocating new space
        _strMallocCpy(&tmpConfigStr, configQueryStr);
        if (tmpConfigStr != NULL)
        {
            new_allConfigsQueryName[configQuery->nConfigQueryName + 1] = tmpConfigStr;

            configQuery->allConfigQueryName = new_allConfigsQueryName;
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

    *((const char **)&(httpRequestAnswer.actionType)) = xml_slave_httpRequestAnswer_str;

    _strMallocCpy(&httpRequestAnswerHeader.content_type, content_type);
    _strMallocCpy(&httpRequestAnswerHeader.content_language, content_language);
    httpRequestAnswer.code = code;
    httpRequestAnswer.headers = httpRequestAnswerHeader;
    _strMallocCpy(&httpRequestAnswer.body, body);

    return httpRequestAnswer;
}

struct xml_slave_log mkLog(
    enum log_level logLevel,
    const char *text)
{
    struct xml_slave_log log;

    *((const char **)&(log.actionType)) = xml_slave_log_str;

    log.logLevel = logLevel;
    _strMallocCpy(&log.text, text);

    return log;
}

struct xml_slave_sqlQueryModifQuery
mkSqlQueryModifyQuery(
    const char *query)
{
    struct xml_slave_sqlQueryModifQuery sqlQueryModifQuery;

    *((const char **)&(sqlQueryModifQuery.actionType)) = xml_slave_sqlQueryModifQuery_str;

    _strMallocCpy(&sqlQueryModifQuery.query, query);

    return sqlQueryModifQuery;
}

struct xml_slave_sqlQuerySelectQuery
mkSqlQuerySelectQuery(
    const char *query)
{
    struct xml_slave_sqlQuerySelectQuery sqlQuerySelectQuery;

    *((const char **)&(sqlQuerySelectQuery.actionType)) = xml_slave_sqlQueryModifQuery_str;

    _strMallocCpy(&sqlQuerySelectQuery.query, query);

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
    int* nElems,
    const char *const rawXml)
{
    const char **tmpBuff = NULL;
    int res = 0;
    int tabLen = 0;

    xmlDocPtr doc = xmlReadMemory(rawXml, strlen(rawXml), "in.xml", NULL, 0);
    if (doc == NULL)
    {
        do_log2("Unable to open document", LOG_LEVEL_ERROR);
        return 1;
    }

    if (xml_apply_xsd(doc, globalData_master_config_xsd) != 0)
    {
        do_log2("parse_xml_master_config: XSD parsing failed", LOG_LEVEL_ERROR);
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

            _strMallocCpy(&((*config)[i].configName), rootNode->name);
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
    xmlDocPtr doc = xmlReadMemory(rawXml, strlen(rawXml), "in.xml", NULL, 0);
    if (doc == NULL)
    {
        do_log2("Unable to open document", LOG_LEVEL_ERROR);
        return 1;
    }

    if (xml_apply_xsd(doc, globalData_master_httpRequest_query_xsd) != 0)
    {
        do_log2("parse_xml_master_httpRequestQuery: XSD parsing failed", LOG_LEVEL_ERROR);
        xmlFreeDoc(doc);
        return 1;
    }

    /* Parsing the file */

    xmlFreeDoc(doc);
}

int parse_xml_master_sqlQueryError(
    struct xml_master_sqlQueryError *sqlQueryError,
    const char *const rawXml)
{
    xmlDocPtr doc = xmlReadMemory(rawXml, strlen(rawXml), "in.xml", NULL, 0);
    if (doc == NULL)
    {
        do_log2("Unable to open document", LOG_LEVEL_ERROR);
        return 1;
    }

    if (xml_apply_xsd(doc, globalData_master_sqlquery_error_xsd) != 0)
    {
        do_log2("parse_xml_master_sqlQueryError: XSD parsing failed", LOG_LEVEL_ERROR);
        xmlFreeDoc(doc);
        return 1;
    }

    /* Parsing the file */

    xmlFreeDoc(doc);
}

int parse_xml_master_sqlQueryModifResult(
    struct xml_master_sqlQueryModifResult *sqlQueryModifResult,
    const char *const rawXml)
{
    xmlDocPtr doc = xmlReadMemory(rawXml, strlen(rawXml), "in.xml", NULL, 0);
    if (doc == NULL)
    {
        do_log2("Unable to open document", LOG_LEVEL_ERROR);
        return 1;
    }

    if (xml_apply_xsd(doc, globalData_master_sqlquery_modif_result_xsd) != 0)
    {
        do_log2("parse_xml_master_sqlQueryModifResult: XSD parsing failed", LOG_LEVEL_ERROR);
        xmlFreeDoc(doc);
        return 1;
    }

    /* Parsing the file */

    xmlFreeDoc(doc);
}

int parse_xml_master_sqlQuerySelectResult(
    struct xml_master_sqlQuerySelectResult *sqlQuerySelectResult,
    const char *const rawXml)
{
    xmlDocPtr doc = xmlReadMemory(rawXml, strlen(rawXml), "in.xml", NULL, 0);
    if (doc == NULL)
    {
        do_log2("Unable to open document", LOG_LEVEL_ERROR);
        return 1;
    }

    if (xml_apply_xsd(doc, globalData_master_sqlquery_select_result_xsd) != 0)
    {
        do_log2("parse_xml_master_sqlQuerySelectResult: XSD parsing failed", LOG_LEVEL_ERROR);
        xmlFreeDoc(doc);
        return 1;
    }

    /* Parsing the file */

    xmlFreeDoc(doc);
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
