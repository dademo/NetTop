#include <stdio.h>
#include <stdlib.h>
#include <microhttpd.h>

#include "engine/tools/shellColors.h"

#include "webserver.h"
#include "engine/router/router.h"
#include "web/test.h"

#include "engine/tools/log.h"

#include "web/config.h"

void dynamicMem();

int main(int argc, char *argv[])
{

  //add_log_target("toto/out.log", LOG_ALL);
  add_log_target("out.log", LOG_ALL | LOG_LEVEL_DEBUG);
  add_log_target("/dev/stderr", LOG_LEVEL_DEBUG | LOG_LEVEL_WARNING | LOG_LEVEL_ERROR);

  do_log("Hello World !", LOG_LEVEL_DEBUG);
  do_log("Hello World !", LOG_LEVEL_INFO);
  do_log("Hello World !", LOG_LEVEL_NOTICE);
  do_log("Hello World !", LOG_LEVEL_WARNING);
  do_log("Hello World !", LOG_LEVEL_ERROR);

  /*
  long double size = 1000;
  for (int i = 0; i < 10; i++)
  {
    size *= 1000;
  }

  struct router_route *allRoutes_ = malloc(size * sizeof(struct router_route));

  struct router_conf routerConf_ = create_router_conf();

  for (int i = 0; i < size; i++)
  {
    create_router_route(allRoutes_ + i, "/toto", 0, &callbackFct);
    add_router_conf(&routerConf_, *(allRoutes_ + i));
  }

  free_router_conf(routerConf_);
  free(allRoutes_);*/

  struct router_conf routerConf = create_router_conf();

  router_add_conf(&routerConf, "/", 0, &callbackFct2);

  load_all_modules(&routerConf);

  router_add_conf(&routerConf, "/toto", 0, &callbackFct);
  router_add_conf(&routerConf, "/tata", 0, &callbackFct);
  router_add_conf(&routerConf, "/tutu", 0, &callbackFct);
  router_add_conf(&routerConf, "/aze", 0, &callbackFct);
  router_add_conf(&routerConf, "/rty", 0, &callbackFct);
  router_add_conf(&routerConf, "/azerty", 0, &callbackFct);
  router_add_conf(&routerConf, "/qsd", 0, &callbackFct);
  router_add_conf(&routerConf, "/fgh", 0, &callbackFct);
  router_add_conf(&routerConf, "/wxc", 0, &callbackFct);
  router_add_conf(&routerConf, "/vbn", 0, &callbackFct);

  runWevServer(routerConf);

  free_router_conf(routerConf);

  free_all_log_target();

  return 0;
}
