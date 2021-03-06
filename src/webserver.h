#ifndef WEBSERVER_H_INCLUDED
#define WEBSERVER_H_INCLUDED

#include "engine/router/router.h"

/**
 * Value that indicate if the server keep running
 */
static int webServer_run = 1;
static regex_t reg_login;
static regex_t reg_login_redirect;
static regex_t reg_logout;
static regex_t reg_logout_redirect;
static regex_t reg_login_status;
static regex_t reg_login_status_redirect;

int handleConnection(void* cls,
                    struct MHD_Connection* connection,
                    const char* url,
                    const char* method,
                    const char* version,
                    const char* upload_data,
                    size_t* upload_data_size,
                    void** con_cls);
                    
int print_out_key (void *cls, enum MHD_ValueKind kind, 
                   const char *key, const char *value);

int runWevServer(struct router_conf);

void handleStop(int signal);

int doRedirect(char* url, struct MHD_Response *response, struct MHD_Connection* connection);

#endif
