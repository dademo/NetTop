#ifndef MOD_LOGIN_H_INCLUDED
#define MOD_LOGIN_H_INCLUDED

#include "../../engine/router/router.h"

int doLogin(
    struct MHD_Response *response,
    struct MHD_Connection *connection,
    void **con_cls);

int doLogout(
    struct MHD_Response *response,
    struct MHD_Connection *connection,
    void **con_cls);

#endif