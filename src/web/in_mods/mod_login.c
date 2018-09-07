#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <microhttpd.h>

#include "mod_login.h"
#include "../../engine/router/router.h"

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

int doLogin(
    struct MHD_Response *response,
    struct MHD_Connection *connection,
    const char *method,
    const char *upload_data,
    size_t *upload_data_size,
    void **con_cls)
{
    int ret;

    if (strcmp(method, MHD_HTTP_METHOD_POST) == 0)
    {

        /* Getting the client infos */
        const union MHD_ConnectionInfo *connInfo = MHD_get_connection_info(connection, MHD_CONNECTION_INFO_CLIENT_ADDRESS);
        struct sockaddr *client = *(struct sockaddr **)connInfo;
        if (client->sa_family == AF_INET)
        {
            struct sockaddr_in *netClient = (struct sockaddr_in *)client;

            /*
        inet_ntop(AF_INET, &(netClient->sin_addr), clientBuffIP, DEFAULT_BUFFER_SIZE);
        clientPort = netClient->sin_port;
        */
        }

        if (IS_LOGGED(*client) == 1)
        {
            response = MHD_create_response_from_buffer(strlen(ALREADY_LOGGED_IN),
                                                       ALREADY_LOGGED_IN,
                                                       MHD_RESPMEM_PERSISTENT);
            ret = MHD_queue_response(connection, MHD_HTTP_OK, response);
            MHD_destroy_response(response);
        }
        else
        {
            addLoggedUser((union netAddress) * client);
            response = MHD_create_response_from_buffer(strlen(LOGIN_OK),
                                                       LOGIN_OK,
                                                       MHD_RESPMEM_PERSISTENT);
            ret = MHD_queue_response(connection, MHD_HTTP_OK, response);
            MHD_destroy_response(response);
        }
        /*
    char *username;
    const char *password = "testpass";
    const char *realm = "test@example.com";
    int ret;

    username = MHD_digest_auth_get_username(connection);

    if (username == NULL)
    {
        response = MHD_create_response_from_buffer(strlen(DENIED),
                                                   DENIED,
                                                   MHD_RESPMEM_PERSISTENT);
        ret = MHD_queue_auth_fail_response(connection, realm,
                                           OPAQUE,
                                           response,
                                           MHD_NO);
        MHD_destroy_response(response);
        return ret;
    }
    ret = MHD_digest_auth_check(connection, realm,
                                username,
                                password,
                                300);
    free(username);
    if ((ret == MHD_INVALID_NONCE) ||
        (ret == MHD_NO))
    {
        response = MHD_create_response_from_buffer(strlen(DENIED),
                                                   DENIED,
                                                   MHD_RESPMEM_PERSISTENT);
        if (NULL == response)
            return MHD_NO;
        ret = MHD_queue_auth_fail_response(connection, realm,
                                           OPAQUE,
                                           response,
                                           (ret == MHD_INVALID_NONCE) ? MHD_YES : MHD_NO);
        MHD_destroy_response(response);
        return ret;
    }
    response = MHD_create_response_from_buffer(strlen(PAGE), PAGE,
                                               MHD_RESPMEM_PERSISTENT);
    ret = MHD_queue_response(connection, MHD_HTTP_OK, response);
    MHD_destroy_response(response);
*/
    }
    else
    {
        response = MHD_create_response_from_buffer(strlen(LOGIN_BAD_METHOD),
                                                   LOGIN_BAD_METHOD,
                                                   MHD_RESPMEM_PERSISTENT);
        ret = MHD_queue_response(connection, MHD_HTTP_METHOD_NOT_ALLOWED, response);
        MHD_destroy_response(response);
    }
    return ret;
}

int doLogout(
    struct MHD_Response *response,
    struct MHD_Connection *connection,
    const char *method,
    const char *upload_data,
    size_t *upload_data_size,
    void **con_cls)
{
    int ret;

    if (strcmp(method, MHD_HTTP_METHOD_POST) == 0)
    {

        /* Getting the client infos */
        const union MHD_ConnectionInfo *connInfo = MHD_get_connection_info(connection, MHD_CONNECTION_INFO_CLIENT_ADDRESS);
        struct sockaddr *client = *(struct sockaddr **)connInfo;
        if (client->sa_family == AF_INET)
        {
            struct sockaddr_in *netClient = (struct sockaddr_in *)client;

            /*
        inet_ntop(AF_INET, &(netClient->sin_addr), clientBuffIP, DEFAULT_BUFFER_SIZE);
        clientPort = netClient->sin_port;
        */
        }

        if (IS_LOGGED(*client) == 1)
        {
            struct login_struct *loggedUser = getLoggedUser((union netAddress) * client);
            delLoggedUser(loggedUser);
            response = MHD_create_response_from_buffer(strlen(LOGIN_OK),
                                                       LOGIN_OK,
                                                       MHD_RESPMEM_PERSISTENT);
            ret = MHD_queue_response(connection, MHD_HTTP_OK, response);
            MHD_destroy_response(response);
        }
        else
        {
            response = MHD_create_response_from_buffer(strlen(ALREADY_LOGGED_OUT),
                                                       ALREADY_LOGGED_OUT,
                                                       MHD_RESPMEM_PERSISTENT);
            ret = MHD_queue_response(connection, MHD_HTTP_OK, response);
            MHD_destroy_response(response);
        }
    }
    else
    {
        response = MHD_create_response_from_buffer(strlen(LOGIN_BAD_METHOD),
                                                   LOGIN_BAD_METHOD,
                                                   MHD_RESPMEM_PERSISTENT);
        ret = MHD_queue_response(connection, MHD_HTTP_METHOD_NOT_ALLOWED, response);
        MHD_destroy_response(response);
    }
    return ret;
}

int doLoginStatus(
    struct MHD_Response *response,
    struct MHD_Connection *connection,
    const char *method,
    void **con_cls)
{
    int ret;

    if (strcmp(method, MHD_HTTP_METHOD_GET) == 0)
    {

        /* Getting the client infos */
        const union MHD_ConnectionInfo *connInfo = MHD_get_connection_info(connection, MHD_CONNECTION_INFO_CLIENT_ADDRESS);
        struct sockaddr *client = *(struct sockaddr **)connInfo;
        if (client->sa_family == AF_INET)
        {
            struct sockaddr_in *netClient = (struct sockaddr_in *)client;

            /*
        inet_ntop(AF_INET, &(netClient->sin_addr), clientBuffIP, DEFAULT_BUFFER_SIZE);
        clientPort = netClient->sin_port;
        */
        }

        if (IS_LOGGED(*client) == 1)
        {
            struct login_struct *loggedUser = getLoggedUser((union netAddress) * client);
            delLoggedUser(loggedUser);
            response = MHD_create_response_from_buffer(strlen(LOGIN_OK),
                                                       LOGIN_OK,
                                                       MHD_RESPMEM_PERSISTENT);
            ret = MHD_queue_response(connection, MHD_HTTP_OK, response);
            MHD_destroy_response(response);
        }
        else
        {
            response = MHD_create_response_from_buffer(strlen(LOGIN_KO),
                                                       LOGIN_KO,
                                                       MHD_RESPMEM_PERSISTENT);
            ret = MHD_queue_response(connection, MHD_HTTP_OK, response);
            MHD_destroy_response(response);
        }
    }
    else
    {
        response = MHD_create_response_from_buffer(strlen(LOGIN_BAD_METHOD),
                                                   LOGIN_BAD_METHOD,
                                                   MHD_RESPMEM_PERSISTENT);
        ret = MHD_queue_response(connection, MHD_HTTP_METHOD_NOT_ALLOWED, response);
        MHD_destroy_response(response);
    }
    return ret;
}

int addLoggedUser(union netAddress user)
{
    struct login_struct newUser;
    newUser.loggedUser = user;
    newUser.lastAction = time(NULL);

    allLoggedUsers = realloc(allLoggedUsers, (allLoggedUsersLen + 1) * sizeof(struct login_struct));
    if (allLoggedUsers == NULL)
    {
        fprintf(stderr, "addLoginKey: Unable to malloc (%d Bytes)\n", (allLoggedUsersLen + 1) * sizeof(struct login_struct));
        return 1;
    }

    memcpy(allLoggedUsers + allLoggedUsersLen, &newUser, sizeof(struct login_struct));

    // Updating the datas
    //conf->all_routes = new_arr;
    allLoggedUsersLen++;

    return 0;
}

int delLoggedUser(struct login_struct *user)
{
    struct login_struct *finalList = malloc((allLoggedUsersLen - 1) * sizeof(struct login_struct));
    int beforeLen = user - allLoggedUsers;
    int afterLen = allLoggedUsersLen - beforeLen - 1;

    if (finalList == NULL)
    {
        fprintf(stderr, "delLoginKey: Unable to malloc size (%d Bytes)\n", (allLoggedUsersLen - 1) * sizeof(struct login_struct));
        return 1;
    }

    memcpy(finalList, allLoggedUsers, beforeLen * sizeof(struct login_struct));

    memcpy(finalList + beforeLen, allLoggedUsers + beforeLen + 1, afterLen * sizeof(struct login_struct));

    free(allLoggedUsers);
    allLoggedUsers = finalList;
    allLoggedUsersLen--;

    return 0;
}

struct login_struct *getLoggedUser(union netAddress user)
{
    for (int i = 0; i < allLoggedUsersLen; i++)
    {
        struct login_struct *currUser = allLoggedUsers + i;
        if (compareAddress(user, currUser->loggedUser) == 0)
        {
            return currUser;
        }
    }
    return NULL;
}

void freeAllLoggedUsers()
{
    if (allLoggedUsersLen > 0)
    {
        free(allLoggedUsers);
    }
    allLoggedUsers = NULL;
    allLoggedUsersLen = 0;
}

void loggedUserUpdateAction(struct login_struct *user)
{
    user->lastAction = time(NULL);
}

int compareAddress(union netAddress left, union netAddress right)
{
    struct sockaddr *_left = (struct sockaddr *)&left;
    struct sockaddr *_right = (struct sockaddr *)&right;

    if (_left->sa_family == _right->sa_family)
    {
        switch (_left->sa_family)
        {
        case AF_INET:
        {
            struct sockaddr_in *__left = (struct sockaddr_in *)_left;
            struct sockaddr_in *__right = (struct sockaddr_in *)_right;

            if (__left->sin_addr.s_addr == __right->sin_addr.s_addr)
            {
                return 0;
            }
            else
            {
                return 1;
            }
            break;
        }

        case AF_INET6:
        {
            struct sockaddr_in6 *__left = (struct sockaddr_in6 *)_left;
            struct sockaddr_in6 *__right = (struct sockaddr_in6 *)_right;

            struct in6_addr leftAdr = __left->sin6_addr;
            struct in6_addr rightAdr = __left->sin6_addr;
            if (__left->sin6_port == __right->sin6_port)
            {
                if (memcmp(&leftAdr, &rightAdr, sizeof(struct in6_addr)) == 0)
                {
                    return 0;
                }
                else
                {
                    return 1;
                }
            }
            else
            {
                return 1;
            }

            break;
        }
        default:
            /* Non-géré */
            return 2;
        }
    }
    else
    {
        return 1;
    }
}