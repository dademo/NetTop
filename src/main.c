#include <stdio.h>
#include <stdlib.h>
#include <microhttpd.h>

#include "webserver.h"
#include "router.h"
#include "web/test.h"

void dynamicMem();

int main(int argc, char *argv[])
{
  //runWevServer();

  //regfree(((struct router_route*) routerConfig._route_config)->urlRegex);
  //regfree(((struct router_route*) routerConfig._route_config + sizeof(struct router_route))->urlRegex);

  struct router_conf routerConf = create_router_conf();

  struct router_route route, route2;

  printf("OK !\n");

  create_router_route(&route, 10);
  create_router_route(&route2, 20);

  add_router_conf(&routerConf, route);
  add_router_conf(&routerConf, route2);

  printf("OK !\n");

  debug_router_conf(routerConf);

  free_router_conf(routerConf);

  return 0;
}

void dynamicMem()
{
  for (int i = 0; i < 10000; i++)
  {
    char *alloc = (char*) malloc(100 * sizeof(char));

    free(alloc);
  }
  return;
}
