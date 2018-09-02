#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <microhttpd.h>

#include "mod_login.h"
#include "../../engine/router/router.h"

#define PAGE "<html><head><title>libmicrohttpd demo</title></head><body>Access granted</body></html>"
#define DENIED "<html><head><title>libmicrohttpd demo</title></head><body>Access denied</body></html>"
#define LOGOUT "<html><head><title>libmicrohttpd demo</title></head><body>Logout</body></html>"
#define ALREADY_LOGOUT "<html><head><title>libmicrohttpd demo</title></head><body>Already logged out</body></html>"
#define OPAQUE "11733b200778ce33060f31c9af70a870ba96ddd4"

int doLogin(
    struct MHD_Response *response,
    struct MHD_Connection *connection,
    void **con_cls)
{
    char *username;
    const char *password = "testpass";
    const char *realm = "test@example.com";
    int ret;

    username = MHD_digest_auth_get_username(connection);
    if (username == NULL)
    {
        response = MHD_create_response_from_buffer(strlen(DENIED),
                                                   DENIED,
                                                   MHD_RESPMEM_PERSISTENT);
        ret = MHD_queue_auth_fail_response(connection, realm,
                                           OPAQUE,
                                           response,
                                           MHD_NO);
        MHD_destroy_response(response);
        return ret;
    }
    ret = MHD_digest_auth_check(connection, realm,
                                username,
                                password,
                                300);
    free(username);
    if ((ret == MHD_INVALID_NONCE) ||
        (ret == MHD_NO))
    {
        response = MHD_create_response_from_buffer(strlen(DENIED),
                                                   DENIED,
                                                   MHD_RESPMEM_PERSISTENT);
        if (NULL == response)
            return MHD_NO;
        ret = MHD_queue_auth_fail_response(connection, realm,
                                           OPAQUE,
                                           response,
                                           (ret == MHD_INVALID_NONCE) ? MHD_YES : MHD_NO);
        MHD_destroy_response(response);
        return ret;
    }
    response = MHD_create_response_from_buffer(strlen(PAGE), PAGE,
                                               MHD_RESPMEM_PERSISTENT);
    ret = MHD_queue_response(connection, MHD_HTTP_OK, response);
    MHD_destroy_response(response);

    return ret;
}