#ifndef ROUTER_H_INCLUDED
#define ROUTER_H_INCLUDED

#include <microhttpd.h>
#include <regex.h>

struct router_conf
{
    struct router_route *all_routes;
    int arr_size;
};

struct router_route
{
    regex_t* route_regex;
    char* (*callbackFct)(const char*);
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
int create_router_route(struct router_route *newRoute, char *strRegex, int ignoreCase, char* (*callbackFct)(const char*));

/**
 * Bind a route and call the appropriate function
 * 
 * @param strRoute The URL request route
 * @param conf The router conf
 * 
 * @return The text given by the callback function; will be returned to the client
 */
char *bind_route(const char *strRoute, struct router_conf conf);

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
char* callbackFct(const char* url);

#endif