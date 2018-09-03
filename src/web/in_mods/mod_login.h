#ifndef MOD_LOGIN_H_INCLUDED
#define MOD_LOGIN_H_INCLUDED

#include <sys/socket.h>
#include <arpa/inet.h>

#include "../../engine/router/router.h"

struct login_key
{
    char key[129];
    time_t lastAction;
};

static struct login_key *allLoginKeys = NULL;
static size_t allLoginKeysLen = 0;

int doLogin(
    struct MHD_Response *response,
    struct MHD_Connection *connection,
    void **con_cls);

struct login_key doGenKey();

int addLoginKey(struct login_key key);

int delLoginKey(struct login_key *key);

struct login_key *getLoginKey(char key[129]);

void clientUpdateAction(char strKey[129]);

void freeAllLoginKeys();

#endif