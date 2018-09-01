#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "router.h"
#include "../tools/regex.h"

/**
 * Create an empty router_conf structure
 * 
 * @return An empty router_conf structure
 */
struct router_conf create_router_conf()
{
    struct router_conf toReturn;
    toReturn.all_routes = NULL;
    toReturn.arr_size = 0;

    return toReturn;
}

/**
 * Freeing a router_conf structure
 * 
 * @param conf The router_conf structure configuration
 */
void free_router_conf(struct router_conf conf)
{
    for (int i = 0; i < conf.arr_size; i++)
    {
        struct router_route *currRoute = conf.all_routes + i;
        regfree(currRoute->route_regex);
        free(currRoute->route_regex);
    }
    free(conf.all_routes);
}

/**
 * Adding a configuration to the rouer_conf structure
 * 
 * @param conf The router_conf structure configuration
 * @param route The route structure to add
 */
int add_router_conf(struct router_conf *conf, struct router_route route)
{
    conf->all_routes = realloc(conf->all_routes, (conf->arr_size + 1) * sizeof(regex_t));

    if (conf->all_routes == NULL)
    {
        fprintf(stderr, "add_router_conf: Unable to malloc (%d Bytes)\n", (conf->arr_size + 1) * sizeof(regex_t));
        return 1;
    }

    // Adding the data
    memcpy(conf->all_routes + conf->arr_size, &route, sizeof(struct router_route));

    // Updating the datas
    //conf->all_routes = new_arr;
    conf->arr_size++;

    return 0;
}

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
int create_router_route(struct router_route *newRoute, const char *strRegex, const int ignoreCase, char *(*callbackFct)(const char *))
{
    if (newRoute == NULL)
    {
        fprintf(stderr, "create_router_route: NULL pointer given for newRoute\n");
        return 1;
    }
    if (strRegex == NULL)
    {
        fprintf(stderr, "create_router_route: NULL pointer given for strRegex\n");
        return 1;
    }

    regex_t *regex = (regex_t *)malloc(sizeof(regex_t));

    if (get_regex(regex, strRegex, ignoreCase) != 0)
    {
        free(regex);
        return 1;
    }

    newRoute->route_regex = regex;
    newRoute->callbackFct = callbackFct;

    return 0;
}

/**
 * Bind a route and call the appropriate function
 * 
 * @param strRoute The URL request route
 * @param conf The router conf
 * 
 * @return The text given by the callback function; will be returned to the client
 */
char *bind_route(const char *strRoute, struct router_conf conf)
{
    for (int i = 0; i < conf.arr_size; i++)
    {
        struct router_route *route = conf.all_routes + i;
        int res = regexec(route->route_regex, strRoute, 0, 0, 0);
        if (res == 0)
        {
            return (*route->callbackFct)(strRoute);
        }
    }
    return NULL;
}

/**
 * The highest level to add a configuration to a router. Will use function of this file to create and add a configuration to the router
 * 
 * @param conf The router conf to populate
 * @param router The string route to add
 * @param caseSensitive If the regex is case sensitive
 * @param callbackFct The callback function used if the route matches
 * @return Return 0 if not error happened, 1 if something appened
 */
int router_add_conf(struct router_conf *conf, const char *route, const int caseSensitive, char *(*callbackFct)(const char *))
{
    struct router_route tmpRoute;
    int res;

    res = create_router_route(&tmpRoute, route, caseSensitive, callbackFct);
    if (res != 0)
    {
        fprintf(stderr, "router_add_conf: Unable to create route\n");
        return res;
    }

    res = add_router_conf(conf, tmpRoute);
    if (res != 0)
    {
        fprintf(stderr, "router_add_conf: Unable to add the route\n");
        return res;
    }

    return 0;
}

/**
 * Debug to test the router conf
 * @param conf The router conf
 */
void debug_router_conf(struct router_conf conf)
{
    fprintf(stderr, "%s", bind_route("/vbn", conf));
}

/**
 * A test callback function
 * 
 * @param url The URL request route
 * 
 * @return A test message
 */
char *callbackFct(const char *url)
{
    printf("Received route ::%s::\n", url);
    return "TOTO\n";
}