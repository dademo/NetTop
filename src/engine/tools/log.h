#ifndef LOG_H_INCLUDED
#define LOG_H_INCLUDED

#define log_dst "./TestC.log"

enum log_error_level
{
    DEBUG,
    NOTICE,
    WARNING,
    ERROR
};

/**
 * Log the given message (if msg is not null)
 * 
 * @param msg The message to log
 * @param level The error level
 */
void do_log(const char *msg, enum log_error_level level);

#endif