#ifndef MOD_LOGIN_H_INCLUDED
#define MOD_LOGIN_H_INCLUDED

#include <sys/socket.h>
#include <arpa/inet.h>

//#include "../../../engine/router/router.h"

#define IS_LOGGED(user) ((getLoggedUser((union netAddress)user) != NULL) ? 1 : 0)

#define ALREADY_LOGGED_IN "Already logged in"
#define ALREADY_LOGGED_OUT "Already logged out"
#define LOGIN_OK "OK"
#define LOGIN_KO "KO"
#define LOGIN_BAD_METHOD "Method not allowed"
#define LOGIN_USAGE "USAGE: POST:\nusername: String\nPassword: String\n"
#define LOGIN_NO "Not logged in"

union netAddress {
    struct sockaddr orig;
    struct sockaddr_in ipv4;
    struct sockaddr_in6 ipv6;
};

struct login_struct
{
    union netAddress loggedUser;
    time_t lastAction;
};

static struct login_struct *allLoggedUsers = NULL;
static size_t allLoggedUsersLen = 0;

int doLogin(
    struct MHD_Response *response,
    struct MHD_Connection *connection,
    const char *method,
    const char *upload_data,
    size_t *upload_data_size,
    void **con_cls);

int doLogout(
    struct MHD_Response *response,
    struct MHD_Connection *connection,
    const char *method,
    const char *upload_data,
    size_t *upload_data_size,
    void **con_cls);

int doLoginStatus(
    struct MHD_Response *response,
    struct MHD_Connection *connection,
    const char *method,
    void **con_cls);


int findUser(char* user, char* password);

int addLoggedUser(union netAddress user);

int delLoggedUser(struct login_struct *user);

struct login_struct *getLoggedUser(union netAddress user);

void loggedUserUpdateAction(struct login_struct *user);

void freeAllLoggedUsers();

int compareAddress(union netAddress left, union netAddress right);

#endif