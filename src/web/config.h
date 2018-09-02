#ifndef WEB_CONFIG_H_INCLUDED
#define WEB_CONFIG_H_INCLUDED

#include "../engine/router/router.h"


/* Adding the different mod files */
#include "mod_login.h"

/* A function that load all the modules */
void load_all_modules(struct router_conf* conf) {
	module_load_login(conf);
}
#endif
