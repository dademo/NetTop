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

    if (spePos == NULL)
    {
        return NULL;
    }

    /* Si ce n'est pas un chemin absolu */
    if (fullPath[0] != '/')
    {
        if (getcwd(cwd, sizeof(cwd)) != NULL)
        {
            strcat(buffer, cwd);
            buffLen = strlen(buffer);
        }
        else
        {
            _do_log(stderr, "do_log: Unable to getCwd()", LOG_LEVEL_ERROR);
            _do_log(stderr, strerror(errno), LOG_LEVEL_ERROR);
            return NULL;
        }
    }

    dirLength = spePos - fullPath;

    if (buffLen != 0 && buffLen + dirLength > PATH_MAX)
    {
        dirLength = PATH_MAX - strlen(buffer);
    }
    
    memcpy(
        buffer + ((buffLen == -1)? 0 : buffLen + 1),
        fullPath,
        dirLength
    );
    if(buffLen >= 0)
    {
        buffer[buffLen] = '/';
    }
    buffer[((buffLen == -1)? 0 : buffLen + 1) + dirLength] = '\0';

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