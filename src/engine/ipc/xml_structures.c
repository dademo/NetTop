#include <stdlib.h>
#include <stdio.h>
#include <string.h>
/* libxml2 */
#include <libxml/tree.h>
/* defs */
#include "xml_structures.h"
/* tools */
#include "ipc_tools.h"
#include "test.h"

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
}

int mkhttpRequestQuery_addPostArg(
    struct xml_master_httpRequestQuery *httpRequestQuery,
    const char *arg_name,
    const char *arg)
{
}

struct xml_master_sqlQueryError
mkSqlQueryError(
    const char *query,
    unsigned int code,
    const char *error)
{
}

struct xml_master_sqlQuerySelectResult_rowValue *
mkSqlQuerySelectResultRowValue(
    struct xml_master_sqlQuerySelectResult *sqlQuerySelectResult,
    unsigned int columnId,
    enum SQLITE_DATA_TYPES dataType,
    const char *data)
{
}

struct xml_master_sqlQuerySelectResult_row
mkSqlQuerySelectResult_addRow()
{
}

int mkSqlQuerySelectResultRowValue_addRowValue(
    struct xml_master_sqlQuerySelectResult_row *sqlQuerySelectResultRow,
    unsigned int columnId,
    const char *dataType,
    const char *data)
{
}

struct xml_master_sqlQuerySelectResult mkSqlQuerySelectResult(
    const char *query)
{
}

/* Slave */
struct xml_slave_configQuery
mkConfigQuery()
{
}

struct xml_slave_configQuery *
addConfigQuery(
    struct xml_slave_configQuery *configQuery,
    const char *configQueryStr)
{
}

struct xml_slave_httpRequestAnswer
mkHttpRequestAnswer(
    const char *content_type,
    const char *content_language,
    unsigned int code,
    const char *body)
{
}

struct xml_slave_log mkLog(
    enum log_level logLevel,
    const char *text)
{
}

struct xml_slave_sqlQueryModifQuery
mkSqlQueryModifyQuery(
    const char *query)
{
}

struct xml_slave_sqlQuerySelectQuery
mkSqlQuerySelectQuery(
    const char *query)
{
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

/* Free */
/* Master */
/* xml_master_config */
void free_xml_master_config(
    struct xml_master_config *config)
{
}

/* xml_master_httpRequestQuery */
void free_xml_master_httpRequestQuery_headers(
    struct xml_master_httpRequestQuery_headers *httpRequestQuery_headers)
{
}

void free_xml_master_httpRequestQuery_postArgs(
    struct xml_master_httpRequestQuery_postArgs *httpRequestQuery_postArgs)
{
}

void free_xml_master_httpRequestQuery(
    struct xml_master_httpRequestQuery *httpRequestQuery)
{
}

/* xml_master_sqlQueryError */
void free_xml_master_sqlQueryError(
    struct xml_master_sqlQueryError *sqlQueryError)
{
}

/* xml_master_sqlQueryModifResult */
void free_xml_master_sqlQueryModifResult(
    struct xml_master_sqlQueryModifResult *sqlQueryModifResult)
{
}

/* xml_master_sqlQuerySelectResult */
void free_xml_master_sqlQuerySelectResult_rowValue(
    struct xml_master_sqlQuerySelectResult_rowValue *sqlQuerySelectResult_rowValue)
{
}

void free_xml_master_sqlQuerySelectResult_row(
    struct xml_master_sqlQuerySelectResult_row *sqlQuerySelectResult_row)
{
}

void free_xml_master_sqlQuerySelectResult(
    struct xml_master_sqlQuerySelectResult *sqlQuerySelectResult)
{
}

/* Slave */
/* xml_slave_configQuery */
void free_xml_slave_configQuery(struct xml_slave_configQuery *configQuery)
{
}

/* xml_slave_httpRequestAnswer */
void free_xml_slave_httpRequestAnswer_headers(struct xml_slave_httpRequestAnswer_headers *httpRequestAndwer_headers)
{
}

void free_xml_slave_httpRequestAnswer(struct xml_slave_httpRequestAnswer *httpRequestAnswer)
{
}

/* xml_slave_log */
void free_xml_slave_log(struct xml_slave_log *log)
{
}

/* xml_slave_sqlQueryModifQuery */
void free_xml_slave_sqlQueryModifQuery(struct xml_slave_sqlQueryModifQuery *sqlQueryModifQuery)
{
}

/* xml_slave_sqlQuerySelectQuery */
void free_xml_slave_sqlQuerySelectQuery(struct xml_slave_sqlQuerySelectQuery *sqlQuerySelectQuery)
{
}
