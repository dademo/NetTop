#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <microhttpd.h>

#include "engine/tools/shellColors.h"

#include "webserver.h"
#include "engine/router/router.h"
#include "web/test.h"

#include "engine/tools/log.h"

#include "engine/out_mods/config.h"

#include "engine/in_mods/mod_login/mod_login.h"

#include "engine/ipc/xml_structures.h"

#include "engine/config/config.h"

#include "engine/ipc/ipc_tools.h"

void dynamicMem();

int main(int argc, char *argv[])
{
  srand(time(NULL));

  readConf("conf.xml");

/*
  const char* out = NULL;
  xmlDocPtr doc = xmlParseFile("conf.xml");
  int res = extractSubDocument(&out, "/config/modules-config/*", doc, 1, "config");
  if (res != 0)
  {
    do_log("main", LOG_LEVEL_ERROR);
  }
  else
  {
    printf("%s\n", out);
  }
  if (out != NULL)
  {
    _FREE(out);
  }
  xmlFreeDoc(doc);
  */

  //add_log_target("toto/out.log", LOG_ALL);
  add_log_target("out.log", LOG_ALL | LOG_LEVEL_DEBUG);
  add_log_target("/dev/stderr", LOG_LEVEL_DEBUG | LOG_LEVEL_WARNING | LOG_LEVEL_ERROR);

  do_log2("Hello World !", LOG_ALL | LOG_LEVEL_DEBUG);

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

  router_add_conf(&routerConf, "/toto", 0, &callbackFct2);
  router_add_conf(&routerConf, "/tata", 0, &callbackFct2);
  router_add_conf(&routerConf, "/tutu", 0, &callbackFct2);
  router_add_conf(&routerConf, "/aze", 0, &callbackFct2);
  router_add_conf(&routerConf, "/rty", 0, &callbackFct2);
  router_add_conf(&routerConf, "/azerty", 0, &callbackFct2);
  router_add_conf(&routerConf, "/qsd", 0, &callbackFct2);
  router_add_conf(&routerConf, "/fgh", 0, &callbackFct2);
  router_add_conf(&routerConf, "/wxc", 0, &callbackFct2);
  router_add_conf(&routerConf, "/vbn", 0, &callbackFct2);

  runWevServer(routerConf);

  free_router_conf(routerConf);

  free_all_log_target();

  freeAllLoggedUsers();

  return 0;
}
