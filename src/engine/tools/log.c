#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <limits.h>
#include <string.h>
#include <time.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <dirent.h>
#include <errno.h>

#include "log.h"
#include "../config.h"
#include "shellColors.h"
#include "tools.h"

/**
 * Log the given message (if msg is not null)
 * 
 * @param msg The message to log
 * @param level The error level
 */
void do_log(const char *msg, enum log_level level)
{
    int done = 0;
    if (allLogTargets != NULL)
    {
        for (int i = 0; i < allLogTargetSize; i++)
        {
            struct log_target *currLogTarget = allLogTargets + i;
            if ((currLogTarget->targetLevel & level) != 0)
            {
                /* Vérification de l'existance du dossier */
                if (currLogTarget->target == NULL)
                {
                    _do_log(stderr, "do_log: NULL value found", LOG_LEVEL_ERROR);
                }
                else
                {
                    struct stat dir_stat;
                    char cwd[PATH_MAX];
                    char *dirName = getAbsoluteDirName(currLogTarget->target, strlen(currLogTarget->target));
                    //printf("%s\n", dirName);
                    if (dirName == NULL)
                    {
                        if (getcwd(cwd, sizeof(cwd)) != NULL)
                        {
                            dirName = cwd;
                        }
                        else
                        {
                            _do_log(stderr, "do_log: Unable to getCwd()", LOG_LEVEL_ERROR);
                            _do_log(stderr, strerror(errno), LOG_LEVEL_ERROR);
                            continue;
                        }
                    }

                    if (stat(dirName, &dir_stat) == 0)
                    { /* OK */

                        if (access(currLogTarget->target, W_OK) == 0 || access(dirName, W_OK) == 0)
                        {
                            FILE *logFile = fopen(currLogTarget->target, "a");

                            if (logFile == NULL)
                            {
                                char errMsg[DEFAULT_BUFFER_SIZE];
                                sprintf(errMsg, "do_log: Unable to open file \"%s\"\n", currLogTarget->target);

                                _do_log(stderr, errMsg, LOG_LEVEL_ERROR);
                                _do_log(stderr, strerror(errno), LOG_LEVEL_ERROR);
                            }
                            else
                            {
                                _do_log(logFile, msg, level & currLogTarget->targetLevel);
                                fflush(logFile);
                                fclose(logFile);
                                done = 1;
                            }
                        }
                        else
                        {
                            char errMsg[DEFAULT_BUFFER_SIZE];
                            sprintf(errMsg, "do_log: Unable to write file \"%s\"\n", currLogTarget->target);

                            _do_log(stderr, errMsg, LOG_LEVEL_ERROR);
                            _do_log(stderr, strerror(errno), LOG_LEVEL_ERROR);
                        }
                    }
                    else
                    { /* KO */
                        char errMsg[DEFAULT_BUFFER_SIZE];
                        sprintf(errMsg, "do_log: Unable to access dir \"%s\"", dirName);

                        _do_log(stderr, errMsg, LOG_LEVEL_ERROR);
                        _do_log(stderr, strerror(errno), LOG_LEVEL_ERROR);
                    }
                }
            }
        }

        if (done == 0)
        {
            _do_log(stderr, msg, level);
        }
    }
    else
    {
        _do_log(stderr, msg, level);
    }

    return;
}

/**
 * Log the given message (if msg is not null)
 * 
 * @param fildes The file descriptor to use
 * @param msg The message to log
 * @param level The error level
 */
void _do_log(FILE *fildes, const char *msg, enum log_level level)
{
    time_t currTime = time(NULL);
    char buffer[DEFAULT_BUFFER_SIZE] = "";
    size_t size = -1;

    size = strftime(buffer, DEFAULT_BUFFER_SIZE, "%F %T", localtime(&currTime));
    if (size <= 0)
    {
        fprintf(stderr, "_do_log: Unable to generate date. Aborting\n");
        return;
    }
#ifdef DEBUG_ENABLED
    if (level & LOG_LEVEL_DEBUG)
    {
        fprintf(fildes, SHELL_FONT_BOLD SHELL_FOREGROUND_BLUE "[%s]" SHELL_FOREGROUND_LIGHT_CYAN " DEBUG:\t%s" SHELL_ALL_RESET "\n", buffer, msg);
    }
#endif
    if (level & LOG_LEVEL_INFO)
    {
        fprintf(fildes, SHELL_FONT_BOLD SHELL_FOREGROUND_BLUE "[%s]" SHELL_FOREGROUND_LIGHT_GREEN " INFO:\t%s" SHELL_ALL_RESET "\n", buffer, msg);
    }
    if (level & LOG_LEVEL_NOTICE)
    {
        fprintf(fildes, SHELL_FONT_BOLD SHELL_FOREGROUND_BLUE "[%s]" SHELL_FOREGROUND_LIGHT_MAGENTA " NOTICE:\t%s" SHELL_ALL_RESET "\n", buffer, msg);
    }
    if (level & LOG_LEVEL_WARNING)
    {
        fprintf(fildes, SHELL_FONT_BOLD SHELL_FOREGROUND_BLUE "[%s]" SHELL_FOREGROUND_LIGHT_YELLOW " WARNING:\t%s" SHELL_ALL_RESET "\n", buffer, msg);
    }
    if (level & LOG_LEVEL_ERROR)
    {
        fprintf(fildes, SHELL_FONT_BOLD SHELL_FOREGROUND_BLUE "[%s]" SHELL_FOREGROUND_LIGHT_RED " ERROR:\t%s" SHELL_ALL_RESET "\n", buffer, msg);
    }

    return;
}

/**
 * Add a target for the log action
 * 
 * @param target The path for the target
 * @param targetLevel The target level to log
 * 
 * @return 0 If everything if OK, 1 if not
 */
int add_log_target(char *target, enum log_level targetLevel)
{
    allLogTargets = realloc(allLogTargets, (allLogTargetSize + 1) * sizeof(struct log_target));

    if (allLogTargets == NULL)
    {
        fprintf(stderr, "add_log_target: Unable to malloc (%d Bytes)\n", (allLogTargetSize + 1) + sizeof(struct log_target *));
        return 1;
    }

    struct log_target tmpTarget;
    int srcLen = strlen(target);

    tmpTarget.target = malloc(sizeof(char) * (srcLen + 1));

    if (tmpTarget.target == NULL)
    {
        fprintf(stderr, "add_log_target: Unable to malloc (%d Bytes)\n", sizeof(char) * (srcLen + 1));
        return 1;
    }

    memcpy(tmpTarget.target, target, srcLen);
    *(tmpTarget.target + srcLen) = '\0';

    tmpTarget.targetLevel = targetLevel;

    memcpy(allLogTargets + allLogTargetSize, &tmpTarget, sizeof(struct log_target));

    allLogTargetSize++;

    return 0;
}

void free_all_log_target()
{
    if (allLogTargets != NULL)
    {
        for (int i = 0; i < allLogTargetSize; i++)
        {
            struct log_target *currTarget = allLogTargets + i;

            free(currTarget->target);
        }
        free(allLogTargets);
    }
}