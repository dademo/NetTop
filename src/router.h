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
    int *val;
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
 */
int add_router_conf(struct router_conf *conf, struct router_route route);

int create_router_route(struct router_route *newRoute, int val);

void debug_router_conf(struct router_conf conf);

#endif