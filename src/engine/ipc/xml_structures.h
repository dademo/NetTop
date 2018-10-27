#ifndef XML_STRUCTURES_H_INCLUDED
#define XML_STRUCTURES_H_INCLUDED

#include "../tools/log.h"

/* Queries string*/
const char *const xml_master_config_str = "config";
const char *const xml_master_httpRequestQuery_str = "httpRequest-query";
const char *const xml_master_sqlQueryError_str = "sqlquery-error";
const char *const xml_master_sqlQueryModifResult_str = "sqlquery-modif-result";
const char *const xml_master_sqlQuerySelectResult_str = "sqlquery-select-result";

const char *const xml_slave_configQuery_str = "config-query";
const char *const xml_slave_httpRequestAnswer_str = "httpRequest-answer";
const char *const xml_slave_log_str = "log";
const char *const xml_slave_sqlQueryModifQuery_str = "sqlquery-modif-query";
const char *const xml_slave_sqlQuerySelectQuery_str = "sqlquery-select-query";

/* SQLite Data Types*/
const char *const SQLITE_INTEGER = "INTEGER";
const char *const SQLITE_TEXT = "TEXT";
const char *const SQLITE_REAL = "REAL";
const char *const SQLITE_BLOB = "BLOB";
const char *const SQLITE_NULL = "NULL";

enum SQLITE_DATA_TYPES
{
    SQLITE_DATA_TYPES_INTEGER,
    SQLITE_DATA_TYPES_TEXT,
    SQLITE_DATA_TYPES_REAL,
    SQLITE_DATA_TYPES_BLOB,
    SQLITE_DATA_TYPES_NULL,
    SQLITE_DATA_TYPES_UNDEFINED
};

/* Log levels */
const char *LOG_LEVEL_DEBUG_STR = "DEBUG";
const char *LOG_LEVEL_INFO_STR = "INFO";
const char *LOG_LEVEL_NOTICE_STR = "NOTICE";
const char *LOG_LEVEL_WARNING_STR = "WARNING";
const char *LOG_LEVEL_ERROR_STR = "ERROR";

/* Structs */
/* Master */

/** xml_master_config **/
/* message.action */
struct xml_master_config
{
    const char *const actionType;
    const char *configName;
    const char *inConfig;
};
/** END xml_master_config **/

/** xml_master_httpRequestQuery **/
/* message.action.query.headers */
struct xml_master_httpRequestQuery_headers
{
    const char *accept;
    const char *accept_charset;
    const char *accept_language;
    const char *user_agent;
};

/* message.action.query.post-args */
struct xml_master_httpRequestQuery_postArgs
{
    const char *arg_name;
    const char *arg;
};

/* message.action.query */
struct xml_master_httpRequestQuery
{
    const char *const actionType;
    const char *queryType;
    const char *path;
    struct xml_master_httpRequestQuery_headers headers;
    unsigned int argLen;
    struct xml_master_httpRequestQuery_postArgs *postArgs;
};
/** END xml_master_httpRequestQuery **/

/* xml_master_sqlQueryError */
/* message.action */
struct xml_master_sqlQueryError
{
    const char *const actionType;
    const char *query;
    unsigned int code;
    const char *error;
};
/** END xml_master_sqlQueryError **/

/** xml_master_sqlQueryModifResult **/
/* message.action */
struct xml_master_sqlQueryModifResult
{
    const char *const actionType;
    const char *query;
    unsigned int changes;
};
/** END xml_master_sqlQueryModifResult **/

/** xml_master_sqlQuerySelectResult **/
/* message.action.answer.row.value[?] */
struct xml_master_sqlQuerySelectResult_rowValue
{
    unsigned int columnId;
    enum SQLITE_DATA_TYPES dataType;
    const char *colName;
    const char *data;
};
/* message.action.answer.row[?] */
struct xml_master_sqlQuerySelectResult_row
{
    unsigned int nCols;
    struct xml_master_sqlQuerySelectResult_rowValue *cols;
};
/* message.action */
struct xml_master_sqlQuerySelectResult
{
    const char *const actionType;
    const char *query;
    unsigned int nRows;
    struct xml_master_sqlQuerySelectResult_row *rows;
};
/** END xml_master_sqlQuerySelectResult **/

/*=* Slave *=*/

/** xml_slave_configQuery **/
/* message.action */
struct xml_slave_configQuery
{
    const char *const actionType;
    unsigned int nConfigQueryName;
    const char *allConfigQueryName;
};
/** END xml_slave_configQuery **/

/** xml_slave_httpRequestAnswer **/
/* message.action.answer.headers */
struct xml_slave_httpRequestAnswer_headers
{
    const char *content_type;
    const char *content_language;
};
/* message.action.answer */
struct xml_slave_httpRequestAnswer
{
    const char *const actionType;
    unsigned int code;
    struct xml_slave_httpRequestAnswer_headers headers;
    const char *body;
};
/** END xml_slave_httpRequestAnswer **/

/** xml_slave_log **/
/* message.action */
struct xml_slave_log
{
    const char *const actionType;
    enum log_level logLevel;
    const char *text;
};
/** END xml_slave_log **/

/** xml_slave_sqlQueryModifQuery **/
struct xml_slave_sqlQueryModifQuery
{
    const char *const actionType;
    const char *query;
};
/** END xml_slave_sqlQueryModifQuery **/

/** xml_slave_sqlQuerySelectQuery **/
struct xml_slave_sqlQuerySelectQuery
{
    const char *const actionType;
    const char *query;
};
/** END xml_slave_sqlQuerySelectQuery **/

/* Functions */
/* Master */

struct xml_master_config
mkMasterConfig(
    const char *configName,
    const char *inConfig);

struct xml_master_httpRequestQuery_headers
mkQueryHeaders(
    const char *accept,
    const char *accept_charset,
    const char *accept_language,
    const char *user_agent);

struct xml_master_httpRequestQuery_postArgs
mkQueryPostArgs(
    const char *arg_name,
    const char *arg);

struct xml_master_httpRequestQuery
mkhttpRequestQuery(
    const char *queryType,
    const char *path,
    const char *accept,
    const char *accept_charset,
    const char *accept_language,
    const char *user_agent);

int
mkhttpRequestQuery_addPostArg(
    struct xml_master_httpRequestQuery *httpRequestQuery,
    const char *arg_name,
    const char *arg);

struct xml_master_sqlQueryError
mkSqlQueryError(
    const char *query,
    unsigned int code,
    const char *error);

struct xml_master_sqlQuerySelectResult_rowValue *
mkSqlQuerySelectResultRowValue(
    struct xml_master_sqlQuerySelectResult *sqlQuerySelectResult,
    unsigned int columnId,
    enum SQLITE_DATA_TYPES dataType,
    const char *data);

struct xml_master_sqlQuerySelectResult_row
mkSqlQuerySelectResult_addRow();

int
mkSqlQuerySelectResultRowValue_addRowValue(
    struct xml_master_sqlQuerySelectResult_row *sqlQuerySelectResultRow,
    unsigned int columnId,
    const char *dataType,
    const char *data);

struct xml_master_sqlQuerySelectResult
mkSqlQuerySelectResult(
    const char *query);

/* Slave */
struct xml_slave_configQuery
mkConfigQuery();

struct xml_slave_configQuery *
addConfigQuery(
    struct xml_slave_configQuery *configQuery,
    const char *configQueryStr);

struct xml_slave_httpRequestAnswer
mkHttpRequestAnswer(
    const char *content_type,
    const char *content_language,
    unsigned int code,
    const char *body);

struct xml_slave_log
mkLog(
    enum log_level logLevel,
    const char *text);

struct xml_slave_sqlQueryModifQuery
mkSqlQueryModifyQuery(
    const char *query);

struct xml_slave_sqlQuerySelectQuery
mkSqlQuerySelectQuery(
    const char *query);

/* Generators */
/* Master */
const char *
stringify_xml_master_config(
    struct xml_master_config config);

const char *
    stringify_xml_master_httpRequestQuery(
        struct xml_master_httpRequestQuery httpRequestQuery);

const char *
stringify_xml_master_sqlQueryError(
    struct xml_master_sqlQueryError sqlQueryError);

const char *
stringify_xml_master_sqlQueryModifResult(
    struct xml_master_sqlQueryModifResult sqlQueryModifResult);

const char *
stringify_xml_master_sqlQuerySelectResult(
    struct xml_master_sqlQuerySelectResult sqlQuerySelectResult);

const char *
stringify_xml_slave_configQuery(
    struct xml_slave_configQuery configQuery);

const char *
stringify_xml_slave_httpRequestAnswer(
    struct xml_slave_httpRequestAnswer httpRequestAnswer);

const char *
stringify_xml_slave_log(struct xml_slave_log log);

const char *
stringify_xml_slave_sqlQueryModifQuery(
    struct xml_slave_sqlQueryModifQuery sqlQueryModifQuery);

const char *
stringify_xml_slave_sqlQuerySelectQuery(
    struct xml_slave_sqlQuerySelectQuery sqlQuerySelectQuery);

/* Free */
/* Master */
/* xml_master_config */
void
free_xml_master_config(
    struct xml_master_config *config);

/* xml_master_httpRequestQuery */
void
free_xml_master_httpRequestQuery_headers(
    struct xml_master_httpRequestQuery_headers *httpRequestQuery_headers);

void
free_xml_master_httpRequestQuery_postArgs(
    struct xml_master_httpRequestQuery_postArgs *httpRequestQuery_postArgs);

void
free_xml_master_httpRequestQuery(
    struct xml_master_httpRequestQuery *httpRequestQuery);

/* xml_master_sqlQueryError */
void
free_xml_master_sqlQueryError(
    struct xml_master_sqlQueryError *sqlQueryError);

/* xml_master_sqlQueryModifResult */
void
free_xml_master_sqlQueryModifResult(
    struct xml_master_sqlQueryModifResult *sqlQueryModifResult);

/* xml_master_sqlQuerySelectResult */
void
free_xml_master_sqlQuerySelectResult_rowValue(
    struct xml_master_sqlQuerySelectResult_rowValue *sqlQuerySelectResult_rowValue);

void
free_xml_master_sqlQuerySelectResult_row(
    struct xml_master_sqlQuerySelectResult_row *sqlQuerySelectResult_row);

void
free_xml_master_sqlQuerySelectResult(
    struct xml_master_sqlQuerySelectResult *sqlQuerySelectResult);

/* Slave */
/* xml_slave_configQuery */
void
free_xml_slave_configQuery(
    struct xml_slave_configQuery *configQuery);

/* xml_slave_httpRequestAnswer */
void
free_xml_slave_httpRequestAnswer_headers(
    struct xml_slave_httpRequestAnswer_headers *httpRequestAndwer_headers);

void
free_xml_slave_httpRequestAnswer(
    struct xml_slave_httpRequestAnswer *httpRequestAnswer);

/* xml_slave_log */
void
free_xml_slave_log(
    struct xml_slave_log *log);

/* xml_slave_sqlQueryModifQuery */
void
free_xml_slave_sqlQueryModifQuery(
    struct xml_slave_sqlQueryModifQuery *sqlQueryModifQuery);

/* xml_slave_sqlQuerySelectQuery */
void
free_xml_slave_sqlQuerySelectQuery(
    struct xml_slave_sqlQuerySelectQuery *sqlQuerySelectQuery);

#endif