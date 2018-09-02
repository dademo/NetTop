#include <stdio.h>
#include <string.h>
#include <unistd.h> // sleep()
#include <signal.h> // signal()
#include <sys/types.h>
#include <sys/select.h>
#include <sys/socket.h>
#include <microhttpd.h>

#include "config.h"
#include "engine/config.h"
#include "webserver.h"
#include "engine/tools/log.h"

/**
 * Value that indicate if the server keep running
 */
int webServer_run = 1;

int runWevServer(struct router_conf routerConf)
{
  // Gestion du signal ...
  signal(SIGHUP, &handleStop);
  signal(SIGINT, &handleStop);
  signal(SIGQUIT, &handleStop);
  signal(SIGTERM, &handleStop);

  struct MHD_Daemon *daemon;

  /*daemon = MHD_start_daemon(MHD_USE_DEBUG | MHD_USE_THREAD_PER_CONNECTION | MHD_USE_SELECT_INTERNALLY, PORT, NULL, NULL,
                            &handleConnection, NULL, MHD_OPTION_END);*/
  daemon = MHD_start_daemon(
    #ifdef DEBUG_ENABLED
      MHD_USE_DEBUG |
    #endif
      MHD_USE_THREAD_PER_CONNECTION |
      MHD_USE_SELECT_INTERNALLY,
    PORT,
    NULL,
    NULL,
    &handleConnection,
    &routerConf,
    MHD_OPTION_END
  );
  if (NULL == daemon)
    return 1;
  else
    //fprintf(stderr, "Server started\n");
  do_log("Server started", LOG_LEVEL_INFO);

  while (webServer_run)
  {
    sleep(1);
  }

  do_log("Stopping daemon", LOG_LEVEL_INFO);

  MHD_stop_daemon(daemon);

  return 0;
}

/**
 * Handle signals :
 *  - SIGHUP
 *  - SIGINT
 *  - SIGQUIT
 *  - SIGTERM
 */
void handleStop(int signal)
{
  switch (signal)
  {
  case SIGHUP:
    do_log("Signal SIGHUP received", LOG_LEVEL_INFO);
    webServer_run = 0;
    break;
  case SIGINT:
    do_log("Signal SIGINT received", LOG_LEVEL_INFO);
    webServer_run = 0;
    break;
  case SIGQUIT:
    do_log("Signal SIGQUIT received", LOG_LEVEL_INFO);
    webServer_run = 0;
    break;
  case SIGTERM:
    do_log("Signal SIGTERM received", LOG_LEVEL_INFO);
    webServer_run = 0;
    break;
  }
}

int handleConnection(void *cls,
                     struct MHD_Connection *connection,
                     const char *url,
                     const char *method,
                     const char *version,
                     const char *upload_data,
                     size_t *upload_data_size,
                     void **con_cls)
{
  struct MHD_Response *response;
  int ret;
  /*
  printf("New %s request for %s using version %s\n", method, url, version);
  if (NULL == *con_cls)
  {
    *con_cls = connection;
    return MHD_YES;
  }*/

  //MHD_get_connection_values(connection, MHD_HEADER_KIND, &print_out_key, NULL);

  //return MHD_NO;
  //return MHD_YES;

  char *bindedResponse = bind_route(url, *((struct router_conf *)cls));

  if (bindedResponse == NULL)
  {
    char buff[DEFAULT_BUFFER_SIZE];
    sprintf(buff, "No route found for \"%s\". Aborting", url);
    do_log(buff, LOG_LEVEL_NOTICE);

    response = MHD_create_response_from_buffer(strlen(ROUTER_BAD_ROUTE_RESPONSE), (void *)ROUTER_BAD_ROUTE_RESPONSE, MHD_RESPMEM_PERSISTENT);

    ret = MHD_queue_response(connection, MHD_HTTP_NOT_FOUND, response);
  }
  else
  {
    //response = MHD_create_response_from_buffer(strlen(toReturn), (void *)toReturn, MHD_RESPMEM_PERSISTENT);
    response = MHD_create_response_from_buffer(strlen(bindedResponse), (void *)bindedResponse, MHD_RESPMEM_PERSISTENT);

    ret = MHD_queue_response(connection, MHD_HTTP_OK, response);
  }
  MHD_destroy_response(response);

  return ret;
}

/*
int
handleConnection (void *cls, struct MHD_Connection *connection,
                      const char *url, const char *method,
                      const char *version, const char *upload_data,
                      size_t *upload_data_size, void **con_cls)
{
  char *user;
  char *pass;
  int fail;
  int ret;
  struct MHD_Response *response;

  if (0 != strcmp (method, MHD_HTTP_METHOD_GET))
    return MHD_NO;
  if (NULL == *con_cls)
    {
      *con_cls = connection;
      return MHD_YES;
    }
  pass = NULL;
  user = MHD_basic_auth_get_username_password (connection, &pass);
  fail = ( (user == NULL) ||
	   (0 != strcmp (user, "root")) ||
	   (0 != strcmp (pass, "pa$$w0rd") ) );  
  if (user != NULL) free (user);
  if (pass != NULL) free (pass);
  if (fail)
    {
      const char *page = "<html><body>Go away.</body></html>";
      response =
	MHD_create_response_from_buffer (strlen (page), (void *) page, 
				       MHD_RESPMEM_PERSISTENT);
      ret = MHD_queue_basic_auth_fail_response (connection,
						"my realm",
						response);
    }
  else
    {
      const char *page = "<html><body>A secret.</body></html>";
      response =
	MHD_create_response_from_buffer (strlen (page), (void *) page, 
				       MHD_RESPMEM_PERSISTENT);
      ret = MHD_queue_response (connection, MHD_HTTP_OK, response);
    }
  MHD_destroy_response (response);
  return ret;
}
*/

int print_out_key(void *cls, enum MHD_ValueKind kind,
                  const char *key, const char *value)
{
  fprintf(stderr, "%s: %s\n", key, value);
  return MHD_YES;
}
