#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <limits.h>
#include <errno.h>

#include "tools.h"
#include "../config.h"
#include "log.h"

void *memrchr(const void *block, char c, size_t size)
{
    for (int i = (size - 1); i >= 0; i--)
    {
        if (*((char *)block + i) == c)
        {
            return (void *)(block + i);
        }
    }
    return NULL;
}

char *getAbsoluteDirName(char *fullPath, size_t size)
{
    static char buffer[PATH_MAX];
    char cwd[PATH_MAX];
    char *spePos = memrchr(fullPath, '/', size);
    int dirLength;
    int buffLen = -1;

    buffer[0] = '\0';
    cwd[0] = '\0';

    /* Si ce n'est pas un chemin absolu */
    if (fullPath[0] != '/')
    {
        char *tmp = getExecLocation();
        if (tmp != NULL)
        {
            char *pos = memrchr(tmp, '/', strlen(tmp));
            memcpy(buffer, tmp, pos - tmp);
        }
        else
        {
            _do_log(stderr, "getAbsoluteDirName: Unable to getExecLocation()", LOG_LEVEL_ERROR);
        }
    }
    else
    {
        memcpy(buffer, fullPath, spePos - fullPath);
        buffer[spePos - fullPath] = '\0';
    }

    return buffer;
}

char *getFileName(char *fullPath, size_t size)
{
    static char buffer[PATH_MAX];
    char *spePos = memrchr(fullPath, '/', size);

    if (spePos == NULL)
    {
        spePos = fullPath;
    }

    int length = size - (spePos - fullPath);

    if (length > (PATH_MAX - 1))
    {
        length = PATH_MAX - 1;
    }

    memcpy(buffer, ++spePos, length);
    buffer[size - length] = '\0';

    return buffer;
}

char *getExecLocation()
{
#ifdef __linux__
    static char path[PATH_MAX];
    ssize_t len = readlink("/proc/self/exe", path, PATH_MAX);
    if (len == -1)
    {
        _do_log(stderr, "getExecLocation: Unable to readlink()", LOG_LEVEL_ERROR);
        _do_log(stderr, strerror(errno), LOG_LEVEL_ERROR);
        return NULL;
    }
    return path;
#endif

    return NULL;
}