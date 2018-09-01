#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

#include "log.h"
#include "../config.h"
#include "shellColors.h"

/**
 * Log the given message (if msg is not null)
 * 
 * @param msg The message to log
 * @param level The error level
 */
void do_log(const char *msg, enum log_level level)
{
    _do_log(stderr, msg, level);
    return;
}

/**
 * Log the given message (if msg is not null)
 * 
 * @param fildes The file descriptor to use
 * @param msg The message to log
 * @param level The error level
 */
void _do_log(FILE* fildes, const char* msg, enum log_level level)
{

    time_t currTime = time(NULL);
    char buffer[DEFAULT_BUFFER_SIZE] = "";
    size_t size = -1;

    size = strftime(buffer, DEFAULT_BUFFER_SIZE, "%F %T", localtime(&currTime));
    if (size <= 0)
    {
        fprintf(stderr, "Unable to generate date. Aborting\n");
        return;
    }

    switch (level)
    {
#ifdef DEBUG_ENABLED
    case LOG_LEVEL_DEBUG:
        fprintf(fildes, SHELL_FONT_BOLD SHELL_FOREGROUND_BLUE "[%s]" SHELL_FOREGROUND_LIGHT_CYAN " DEBUG:\t%s" SHELL_ALL_RESET "\n", buffer, msg);
        break;
#endif
    case LOG_LEVEL_NOTICE:
        fprintf(fildes, SHELL_FONT_BOLD SHELL_FOREGROUND_BLUE "[%s]" SHELL_FOREGROUND_LIGHT_GREEN " NOTICE:\t%s" SHELL_ALL_RESET "\n", buffer, msg);
        break;
    case LOG_LEVEL_WARNING:
        fprintf(fildes, SHELL_FONT_BOLD SHELL_FOREGROUND_BLUE "[%s]" SHELL_FOREGROUND_LIGHT_MAGENTA " WARNING:\t%s" SHELL_ALL_RESET "\n", buffer, msg);
        break;
    case LOG_LEVEL_ERROR:
        fprintf(fildes, SHELL_FONT_BOLD SHELL_FOREGROUND_BLUE "[%s]" SHELL_FOREGROUND_LIGHT_RED " ERROR:\t%s" SHELL_ALL_RESET "\n", buffer, msg);
        break;
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
    return 0;
}