#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <microhttpd.h>

#include "mod_login.h"
#include "../../engine/router/router.h"

#define PAGE "<html><head><title>libmicrohttpd demo</title></head><body>Access granted</body></html>"
#define DENIED "<html><head><title>libmicrohttpd demo</title></head><body>Access denied</body></html>"
#define LOGOUT "<html><head><title>libmicrohttpd demo</title></head><body>Logout</body></html>"
#define ALREADY_LOGOUT "<html><head><title>libmicrohttpd demo</title></head><body>Already logged out</body></html>"
#define OPAQUE "11733b200778ce33060f31c9af70a870ba96ddd4"

int doLogin(
    struct MHD_Response *response,
    struct MHD_Connection *connection,
    void **con_cls)
{
    doGenKey();
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

    return ret;
}

struct login_key doGenKey()
{
    struct login_key key;
    char strKey[129];

    for (int i = 0; i < 128; i++)
    {
        int tmpVal = -1;
        int _continue = 1;

        while (_continue == 1)
        {
            tmpVal = rand() % 128;
            if (
                /*(tmpVal >= 'a' && tmpVal <= 'z') ||
                (tmpVal >= 'A' && tmpVal <= 'Z')*/
                tmpVal >= 33 && tmpVal <= 126)
            {
                _continue = 0;
            }
        }

        key.key[i] = tmpVal;
    }
    key.key[128] = '\0';
    key.lastAction = time(NULL);

    return key;
}

int addLoginKey(struct login_key key)
{
    allLoginKeys = realloc(allLoginKeys, (allLoginKeysLen + 1) * sizeof(struct login_key));
    if (allLoginKeys == NULL)
    {
        fprintf(stderr, "addLoginKey: Unable to malloc (%d Bytes)\n", (allLoginKeysLen + 1) * sizeof(struct login_key));
        return 1;
    }

    memcpy(allLoginKeys + allLoginKeysLen, &key, sizeof(struct login_key));

    // Updating the datas
    //conf->all_routes = new_arr;
    allLoginKeysLen++;

    return 0;
}

int delLoginKey(struct login_key *key)
{
    struct login_key* finalList = malloc((allLoginKeysLen - 1) * sizeof(struct login_key));
    int beforeLen = key - allLoginKeys;
    int afterLen = allLoginKeysLen - beforeLen - 1;

    if(finalList == NULL)
    {
        fprintf(stderr, "delLoginKey: Unable to malloc size (%d Bytes)\n", (allLoginKeysLen - 1) * sizeof(struct login_key));
        return 1;
    }

    memcpy(finalList, allLoginKeys, beforeLen * sizeof(struct login_key));

    memcpy(finalList + beforeLen, allLoginKeys + beforeLen + 1, afterLen * sizeof(struct login_key));

    free(allLoginKeys);
    allLoginKeys = finalList;
    allLoginKeysLen--;

    return 0;
}

struct login_key *getLoginKey(char key[129])
{
    for (int i = 0; i < allLoginKeysLen; i++)
    {
        struct login_key *currKey = allLoginKeys + i;
        if (strcmp(currKey->key, key) == 0)
        {
            return currKey;
        }
    }
    return NULL;
}

void clientUpdateLoginKeyAction(char strKey[129])
{
    struct login_key* key = getLoginKey(strKey);
    key->lastAction = time(NULL);
}

void freeAllLoginKeys()
{
    free(allLoginKeys);
    allLoginKeysLen = 0;
}