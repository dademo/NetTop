#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <microhttpd.h>

#include "mod_noRoute.h"


int noRoute(
    const char *url,
    struct MHD_Response *response,
    struct MHD_Connection *connection,
    void **con_cls)
{
    char retText[1024];
    int ret = 0;

    sprintf(retText, NOROUTE_BODY, "No route found", url);

    response = MHD_create_response_from_buffer(strlen(retText), (void *)retText, MHD_RESPMEM_PERSISTENT);
    ret = MHD_queue_response(connection, MHD_HTTP_MOVED_PERMANENTLY, response);

    MHD_destroy_response(response);

    return MHD_HTTP_NOT_FOUND;
}