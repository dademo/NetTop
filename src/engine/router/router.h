#ifndef ROUTER_H_INCLUDED
#define ROUTER_H_INCLUDED

#include <regex.h>

struct router_conf
{
    struct router_route *all_routes;
    int arr_size;
};

struct router_route
{
    regex_t *route_regex;
    int (*callbackFct)(
        const char *,
        struct MHD_Response *,
        struct MHD_Connection *,
        const char *upload_data,
        size_t *upload_data_size,
        void **);
};

enum route_status
{
    route_status_NO = 0,
    route_status_OK = 1,
    route_status_KO = 2
};

/**
 * Create an empty router_conf structure
 * 
 * @return An empty router_conf structure
 */
struct router_conf create_router_conf();

/**
 * Freeing a router_conf structure
 * 
 * @param conf The router_conf structure configuration
 */
void free_router_conf(struct router_conf conf);

/**
 * Adding a configuration to the rouer_conf structure
 * 
 * @param conf The router_conf structure configuration
 * @param route The route structure to add
 * 
 * @return Return 0 if not error happened, 1 if something appened
 */
int add_router_conf(struct router_conf *conf, struct router_route route);

/**
 * Creating a configuration we can add the a router
 * 
 * @param newRoute The route config we will fill
 * @param strRegex The regex string to use
 * @param ignoreCase To enable or not the case sensitivity
 * @param callbackFct The callback function to call if the route match
 * 
 * @return Return 0 if not error happened, 1 if something appened
 */
int create_router_route(struct router_route *newRoute, const char *strRegex, const int ignoreCase, int (*callbackFct)(const char *, struct MHD_Response *, struct MHD_Connection *, const char *, size_t *, void **));

/**
 * Bind a route and call the appropriate function
 * 
 * @param strRoute The URL request route
 * @param conf The router conf
 * 
 * @return The ret value
 */
int bind_route(enum route_status *status, const char *strRoute, struct MHD_Response *response, struct MHD_Connection *connection, const char *upload_data, size_t *upload_data_size, void **con_cls, struct router_conf conf);

/**
 * The highest level to add a configuration to a router. Will use function of this file to create and add a configuration to the router
 * 
 * @param conf The router conf to populate
 * @param router The string route to add
 * @param caseSensitive If the regex is case sensitive
 * @param callbackFct The callback function used if the route matches
 * @return Return 0 if not error happened, 1 if something appened
 */
int router_add_conf(struct router_conf *conf, const char *rMHD_create_response_from_bufferoute, const int caseSensitive, int (*callbackFct)(const char *, struct MHD_Response *, struct MHD_Connection *, const char *, size_t *, void **));

/**
 * Debug to test the router conf
 * @param conf The router conf
 */
void debug_router_conf(struct router_conf conf);

/**
 * A test callback function
 * 
 * @param url The URL request route
 * 
 * @return A test message
 */
char *callbackFct(const char *url);

int callbackFct2(
    const char *url,
    struct MHD_Response *response,
    struct MHD_Connection *connection,
    const char *upload_data,
    size_t *upload_data_size,
    void **con_cls);

#endif