#include <stdio.h>
#include <string.h>
#include <unistd.h> // sleep()
#include <signal.h> // signal()
#include <regex.h>
#include <sys/types.h>
#include <sys/select.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <microhttpd.h>

#include "config.h"
#include "engine/config.h"
#include "engine/tools/log.h"
#include "engine/in_mods/mod_login/mod_login.h"
#include "engine/tools/regex.h"

#include "webserver.h"

int runWevServer(struct router_conf routerConf)
{
  struct MHD_Daemon *daemon;

  // Gestion du signal ...
  signal(SIGHUP, &handleStop);
  signal(SIGINT, &handleStop);
  signal(SIGQUIT, &handleStop);
  signal(SIGTERM, &handleStop);

  if (get_regex(&reg_login, "^/login$", 1) != 0)
  {
    do_log2("Unable to compile the login regex. Closing the program", LOG_LEVEL_ERROR);
    return 1;
  }
  if (get_regex(&reg_login_redirect, "^/login.+", 1) != 0)
  {
    do_log2("Unable to compile the login regex. Closing the program", LOG_LEVEL_ERROR);
    return 1;
  }
  if (get_regex(&reg_logout, "^/logout$", 1) != 0)
  {
    do_log2("Unable to compile the logout regex. Closing the program", LOG_LEVEL_ERROR);
    return 1;
  }
  if (get_regex(&reg_logout_redirect, "^/logout.+", 1) != 0)
  {
    do_log2("Unable to compile the logout regex. Closing the program", LOG_LEVEL_ERROR);
    return 1;
  }
  if (get_regex(&reg_login_status, "^/login/status$", 1) != 0)
  {
    do_log2("Unable to compile the login status regex. Closing the program", LOG_LEVEL_ERROR);
    return 1;
  }
  if (get_regex(&reg_login_status_redirect, "^/login/status.+", 1) != 0)
  {
    do_log2("Unable to compile the login status regex. Closing the program", LOG_LEVEL_ERROR);
    return 1;
  }
  

  /*daemon = MHD_start_daemon(MHD_USE_DEBUG | MHD_USE_THREAD_PER_CONNECTION | MHD_USE_SELECT_INTERNALLY, PORT, NULL, NULL,
                            &handleConnection, NULL, MHD_OPTION_END);*/
  daemon = MHD_start_daemon(
#ifdef DEBUG
      MHD_USE_DEBUG |
#endif
          MHD_USE_THREAD_PER_CONNECTION |
          MHD_USE_SELECT_INTERNALLY,
      PORT,
      NULL,
      NULL,
      &handleConnection,
      &routerConf,
      MHD_OPTION_END);
  if (NULL == daemon)
    return 1;
  else
    do_log2("Server started", LOG_LEVEL_INFO | LOG_LEVEL_DEBUG);

  while (webServer_run)
  {
    sleep(1);
  }

  do_log2("Stopping daemon", LOG_LEVEL_INFO | LOG_LEVEL_DEBUG);

  static regex_t reg_logout;
  static regex_t reg_logout_redirect;
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
    do_log2("Signal SIGHUP received", LOG_LEVEL_INFO);
    webServer_run = 0;
    break;
  case SIGINT:
    do_log2("Signal SIGINT received", LOG_LEVEL_INFO);
    webServer_run = 0;
    break;
  case SIGQUIT:
    do_log2("Signal SIGQUIT received", LOG_LEVEL_INFO);
    webServer_run = 0;
    break;
  case SIGTERM:
    do_log2("Signal SIGTERM received", LOG_LEVEL_INFO);
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

  char clientBuffIP[DEFAULT_BUFFER_SIZE] = "";
  int clientPort = -1;
  char msgBuff[DEFAULT_BUFFER_SIZE] = "";
  const union MHD_ConnectionInfo *connInfo;
  struct sockaddr *client;
  char *bindedResponse;

  /* Allowing only GET requests */
  if (0 != strcmp(method, MHD_HTTP_METHOD_GET) && 0 != strcmp(method, MHD_HTTP_METHOD_POST))
  {
    return MHD_NO;
  }

  /* Getting the client infos */
  connInfo = MHD_get_connection_info(connection, MHD_CONNECTION_INFO_CLIENT_ADDRESS);
  client = *(struct sockaddr **)connInfo;

  if (client->sa_family == AF_INET)
  {
    struct sockaddr_in *netClient = (struct sockaddr_in *)client;
    inet_ntop(AF_INET, &(netClient->sin_addr), clientBuffIP, DEFAULT_BUFFER_SIZE);
    clientPort = netClient->sin_port;
  }
  if (client->sa_family == AF_INET6)
  {
    struct sockaddr_in6 *netClient = (struct sockaddr_in6 *)client;
    inet_ntop(AF_INET6, &(netClient->sin6_addr), clientBuffIP, DEFAULT_BUFFER_SIZE);
    clientPort = netClient->sin6_port;
  }

  if (regexec(&reg_login_status_redirect, url, 0, 0, 0) == 0)
  {
    ret = doRedirect("/login/status", response, connection);
  }
  else if (regexec(&reg_login_status, url, 0, 0, 0) == 0)
  {
    ret = doLoginStatus(response, connection, method, con_cls);
  }
  else if (regexec(&reg_login_redirect, url, 0, 0, 0) == 0)
  {
    ret = doRedirect("/login", response, connection);
  }
  else if (regexec(&reg_login, url, 0, 0, 0) == 0)
  {
    ret = doLogin(response, connection, method, upload_data, upload_data_size, con_cls);
  }
  else if (regexec(&reg_logout_redirect, url, 0, 0, 0) == 0)
  {
    ret = doRedirect("/logout", response, connection);
  }
  else if (regexec(&reg_logout, url, 0, 0, 0) == 0)
  {
    ret = doLogout(response, connection, method, upload_data, upload_data_size, con_cls);
  }
  else
  {
    //bindedResponse = bind_route(url, *((struct router_conf *)cls));
    enum route_status status = route_status_NO;
    ret = bind_route(&status, url, response, connection, upload_data, upload_data_size, con_cls, *((struct router_conf *)cls));

    if (strlen(clientBuffIP) > 0)
    {
      if (ret == route_status_OK)
      {
        sprintf(msgBuff, "Request: %s\t%d\tOK", clientBuffIP, clientPort);
        do_log2(msgBuff, LOG_LEVEL_INFO | LOG_LEVEL_DEBUG);
      }
      else
      {
        sprintf(msgBuff, "Request: %s\t%d\tno_route (%s)", clientBuffIP, clientPort, url);
        do_log2(msgBuff, LOG_LEVEL_NOTICE | LOG_LEVEL_DEBUG);
      }
    }

    /*if (bindedResponse == NULL)
    {
      char buff[DEFAULT_BUFFER_SIZE];

      response = MHD_create_response_from_buffer(strlen(ROUTER_BAD_ROUTE_RESPONSE), (void *)ROUTER_BAD_ROUTE_RESPONSE, MHD_RESPMEM_PERSISTENT);

      ret = MHD_queue_response(connection, MHD_HTTP_NOT_FOUND, response);

      if (strlen(clientBuffIP) > 0)
      {
        sprintf(msgBuff, "Request: %s\t%d\tno_route (%s)", clientBuffIP, clientPort, url);
        do_log2(msgBuff, LOG_LEVEL_NOTICE | LOG_LEVEL_DEBUG);
      }
    }
    else
    {
      //response = MHD_create_response_from_buffer(strlen(toReturn), (void *)toReturn, MHD_RESPMEM_PERSISTENT);
      response = MHD_create_response_from_buffer(strlen(bindedResponse), (void *)bindedResponse, MHD_RESPMEM_PERSISTENT);

      ret = MHD_queue_response(connection, MHD_HTTP_OK, response);

      

      if (strlen(clientBuffIP) > 0)
      {
        sprintf(msgBuff, "Request: %s\t%d\tOK", clientBuffIP, clientPort);
        do_log2(msgBuff, LOG_LEVEL_INFO | LOG_LEVEL_DEBUG);
      }
    }*/
    //MHD_destroy_response(response);
  }

  return ret;
}

int print_out_key(void *cls, enum MHD_ValueKind kind,
                  const char *key, const char *value)
{
  fprintf(stderr, "%s: %s\n", key, value);
  return MHD_YES;
}

int doRedirect(char *target, struct MHD_Response *response, struct MHD_Connection *connection)
{
  int ret = 0;
  /* https://stackoverflow.com/questions/24386128/implement-http-302-response-with-libmicrohttpd-in-c */
  response = MHD_create_response_from_buffer(strlen(REDIRECT), (void *)REDIRECT, MHD_RESPMEM_PERSISTENT);
  MHD_add_response_header(response, "Location", target);
  ret = MHD_queue_response(connection, MHD_HTTP_PERMANENT_REDIRECT, response);

  MHD_destroy_response(response);

  return ret;
}