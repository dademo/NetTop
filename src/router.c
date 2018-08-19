#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "router.h"

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
    printf("arr_size: %d\n", conf.arr_size);
    for (int i = 0; i < conf.arr_size; i++)
    {
        struct router_route *currRoute = (struct router_route *)conf.all_routes + (i * sizeof(struct router_route));
        free(currRoute->val);
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
    struct router_route *new_arr = (struct router_route *)malloc((conf->arr_size + 1) * sizeof(struct router_route));

    if (new_arr == NULL)
    {
        printf("add_router_conf: Unable to malloc (%d)\n", (conf->arr_size + 1) * sizeof(struct router_route));
        return 1;
    }

    if (conf->arr_size > 0)
    {
        // Copying the old datas
        memcpy(new_arr, conf->all_routes, conf->arr_size * sizeof(struct router_route));
    }

    // Adding the data
    memcpy(new_arr + (conf->arr_size * sizeof(struct router_route)), &route, sizeof(struct router_route));

    if (conf->arr_size > 0)
    {
        // Freeing the old datas
        free(conf->all_routes);
    }

    // Updating the datas
    conf->all_routes = new_arr;
    conf->arr_size++;
    printf("%d\n", conf->arr_size);
    return 0;
}

int create_router_route(struct router_route *newRoute, int val)
{
    if (newRoute == NULL)
    {
        printf("NULL pointer given for newRoute\n");
        return 1;
    }

    newRoute->val = (int *)malloc(sizeof(int));
    if (newRoute->val == NULL)
    {
        printf("create_router_route: Unable to malloc (%d)\n", sizeof(int));
        return 1;
    }

    *(newRoute->val) = val;

    return 0;
}


void debug_router_conf(struct router_conf conf)
{
    for (int i = 0; i < conf.arr_size; i++)
    {
        struct router_route* currRoute = conf.all_routes + (i * sizeof(struct router_route));
        printf("value: %d <> %d\n", i, *(currRoute->val));
    }
}