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
void do_log(const char *msg, enum log_error_level level)
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
    case DEBUG:
        fprintf(stderr, SHELL_FONT_BOLD SHELL_FOREGROUND_BLUE "[%s]" SHELL_FOREGROUND_LIGHT_CYAN " DEBUG: %s" SHELL_ALL_RESET "\n", buffer, msg);
        break;
#endif
    case NOTICE:
        fprintf(stderr, SHELL_FONT_BOLD SHELL_FOREGROUND_BLUE "[%s]" SHELL_FOREGROUND_LIGHT_GREEN " NOTICE:\t%s" SHELL_ALL_RESET "\n", buffer, msg);
        break;
    case WARNING:
        fprintf(stderr, SHELL_FONT_BOLD SHELL_FOREGROUND_BLUE "[%s]" SHELL_FOREGROUND_LIGHT_MAGENTA " WARNING:\t%s" SHELL_ALL_RESET "\n", buffer, msg);
        break;
    case ERROR:
        fprintf(stderr, SHELL_FONT_BOLD SHELL_FOREGROUND_BLUE "[%s]" SHELL_FOREGROUND_LIGHT_RED " ERROR:\t%s" SHELL_ALL_RESET "\n", buffer, msg);
        break;
    }

    return;
}