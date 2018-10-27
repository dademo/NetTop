#ifndef XML_MESSAGES_MASTER_H_INCLUDED
#define XML_MESSAGES_MASTER_H_INCLUDED

#include "xml_structures.h"

/* Structure manipulators */
void xml_master_config_free();

struct xml_master_config getConfig(char* inMsg);


#endif