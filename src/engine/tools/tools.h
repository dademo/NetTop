#ifndef TOOLS_H_INCLUDED
#define TOOLS_H_

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>


void *memrchr(const void *block, char c, size_t size);

char *getAbsoluteDirName(char *fullPath, size_t size);
char *getFileName(char *fullPath, size_t size);

char *getExecLocation();

#endif