#include <stdlib.h>
#include <stdio.h>

#include "mod_login.h"
#include "../engine/router/router.h"

void module_load_login(struct router_conf *conf)
{
    router_add_conf(conf, "/login", 0, &doLogin);
}

char *doLogin(const char* url)
{
    return "<head><title>LOGIN</title></head><body><h1>LOGIN</h1></body>";
}