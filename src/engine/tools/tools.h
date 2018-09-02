#ifndef TOOLS_H_INCLUDED
#define TOOLS_H_

void *memrchr(const void *block, char c, size_t size);

char *getAbsoluteDirName(char *fullPath, size_t size);
char *getFileName(char *fullPath, size_t size);

#endif