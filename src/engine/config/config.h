#ifndef CONFIG_H_INCLUDED
#define CONFIG_H_INCLUDED

enum conf_target_type {
    target_ptr, /* Internal module */
    target_ipc  /* External module */
};

struct conf_target_ipc {
    int pipe[2];
}

struct config_target {
    char* config_key;
    enum conf_target_type config_target_type;
};

#endif