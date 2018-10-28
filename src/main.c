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

char rawXML[] = "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\
<message>\
    <action type=\"config\">\
        <CONFIG1>\
            <sub_elem1>TOTO</sub_elem1>\
            <sub_elem2>TATA</sub_elem2>\
        </CONFIG1>\
    </action>\
</message>";

char rawXML2[] = "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\
<message>\
    <action type=\"httpRequest-query\">\
        <query type=\"POST\">\
            <path>/toto</path>\
            <headers>\
                <Accept>text/html</Accept>\
                <Accept-Charset>utf-8</Accept-Charset>\
                <Accept-Language>en-US</Accept-Language>\
                <User-Agent>Mozilla/5.0 (X11; Linux x86_64…) Gecko/20100101 Firefox/60.0</User-Agent>\
            </headers>\
            <post-args>\
                <arg name=\"toto\">aze</arg>\
            </post-args>\
        </query>\
    </action>\
</message>";

char rawXML3[] = "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\
<message>\
    <action type=\"sqlquery-select-result\">\
        <query>\
            SELECT\
                *\
            FROM test\
            ;\
        </query>\
        <answer>\
            <row>\
                <value columnId=\"1\" type=\"INTEGER\" name=\"id\">1</value>\
                <value columnId=\"2\" type=\"TEXT\" name=\"First Name\">TOTO</value>\
                <value columnId=\"3\" type=\"REAL\" name=\"Last Name\">TUTU</value>\
            </row>\
            <row>\
                <value columnId=\"1\" type=\"BLOB\" name=\"id\">2</value>\
                <value columnId=\"2\" type=\"NULL\" name=\"First Name\"></value>\
                <value columnId=\"3\" type=\"TEXT\" name=\"Last Name\">TITI</value>\
            </row>\
        </answer>\
    </action>\
</message>";

void dynamicMem();

int main(int argc, char *argv[])
{
  srand(time(NULL));

  //readConf("conf.xml");

  /*
  const char **out = NULL;
  int tabLen = 0;
  xmlDocPtr doc = xmlParseFile("conf.xml");
  //int res = extractSubDocument(&out, "/config/modules-config/*", doc, 1, "config");
  int res = extractSubDocumentMulti(&out, &tabLen, "/config/modules-config/*", doc, 1);
printf("res: %d\n", res);
  if (res != 0)
  {
    do_log2("main", LOG_LEVEL_ERROR);
  }
  else
  {
    for (int i = 0; i < tabLen; i++)
    {
      printf("%s\n", out[i]);
    }
  }
  if (out != NULL)
  {
    for (int i = 0; i < tabLen; i++)
    {
      _FREE(out[i]);
    }
    _FREE(out);
  }
  xmlFreeDoc(doc);
  */

  int res = 0;
  int nElems;
  struct xml_master_config *allConfigs;

  res = parse_xml_master_config(&allConfigs, &nElems, rawXML);

  if (res == 0)
  {
    for (int i = 0; i < nElems; i++)
    {
      printf("%s:%s\n", allConfigs->configName, allConfigs->inConfig);
      free_xml_master_config(allConfigs + i);
    }
  }

  struct xml_master_httpRequestQuery requestQuery;

  res = parse_xml_master_httpRequestQuery(&requestQuery, rawXML2);

  if (res == 0)
  {
    printf("%s\n", requestQuery.path);

    free_xml_master_httpRequestQuery(&requestQuery);
  }
  else
  {
    printf("KO\n");
  }

  struct xml_master_sqlQuerySelectResult sqlQuerySelectResult;

  res = parse_xml_master_sqlQuerySelectResult(&sqlQuerySelectResult, rawXML3);
  if (res == 0)
  {
    printf("%s\n", sqlQuerySelectResult.query);
    printf("%d\n", sqlQuerySelectResult.nRows);

    for (int i = 0; i < sqlQuerySelectResult.nRows; i++)
    {
      printf("row %d, %d cols\n", i, sqlQuerySelectResult.rows[i].nCols);
      for(int j = 0; j < sqlQuerySelectResult.rows[i].nCols; j++) {
        printf("Col %d :\n\t%d\n\t%d\n\t%s\n\t%s\n", j,
        sqlQuerySelectResult.rows[i].cols[j].columnId,
        sqlQuerySelectResult.rows[i].cols[j].dataType,
        sqlQuerySelectResult.rows[i].cols[j].colName,
        sqlQuerySelectResult.rows[i].cols[j].data);
      }
    }

    free_xml_master_sqlQuerySelectResult(&sqlQuerySelectResult);
  }
  else
  {
    printf("KO\n");
  }

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
