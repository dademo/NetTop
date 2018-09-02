#ifndef LOGIN_H_INCLUDED
#define LOGIN_H_INCLUDED

#include "../engine/router/router.h"

void module_load_login(struct router_conf *conf);

char *doLogin(const char *url);

#endif