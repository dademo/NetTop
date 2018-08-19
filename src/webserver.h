#ifndef WEBSERVER_H_INCLUDED
#define WEBSERVER_H_INCLUDED

int handleConnection(void* cls,
                    struct MHD_Connection* connection,
                    const char* url,
                    const char* method,
                    const char* version,
                    const char* upload_data,
                    size_t* upload_data_size,
                    void** con_cls);
int print_out_key (void *cls, enum MHD_ValueKind kind, 
                   const char *key, const char *value);

int runWevServer();

void handleStop(int signal);

#endif
