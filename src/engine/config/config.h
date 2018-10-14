#ifndef ENGINE_CONFIG_CONFIG_H_INCLUDED
#define ENGINE_CONFIG_CONFIG_H_INCLUDED

#include <libxml/tree.h>

struct conf_target_ipc {
    int pOut[2]; /* Pipe for external communication */
    int pIn[2]; /* Pipe for incoming data */
};

struct config_target {
    char* config_key;
    struct conf_target_ipc target;
};

struct arr_config_target {
    int length;
    struct config_target* target;
};

struct xmlConfig {
    char* fileName;
    xmlDocPtr doc;
};


int addConfigTarget(char* config_key, struct conf_target_ipc target, struct arr_config_target* config_target);

int delConfigTarget(struct arr_config_target* all_config_target, struct config_target config_target);

void cleanConfig_target(struct arr_config_target config_target);

struct conf_target_ipc* getIPCTarget(struct arr_config_target all_config_target, char *config_key);

void readConf(char* confPath);

char* getModuleDocument(xmlDocPtr doc, char* moduleName);

#endif