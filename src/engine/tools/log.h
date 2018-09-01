#ifndef LOG_H_INCLUDED
#define LOG_H_INCLUDED

#define log_dst "./TestC.log"


enum log_level
{
    LOG_LEVEL_DEBUG = 1,
    LOG_LEVEL_INFO = 2,
    LOG_LEVEL_NOTICE = 4,
    LOG_LEVEL_WARNING = 8,
    LOG_LEVEL_ERROR = 16
};

#define LOG_ALL LOG_LEVEL_INFO | LOG_LEVEL_NOTICE | LOG_LEVEL_WARNING | LOG_LEVEL_ERROR

struct log_target {
    enum log_level targetLevel;
    char* target;
};


static struct log_target* allLogTargets = NULL;
static int allLogTargetSize = 0;

/**
 * Log the given message (if msg is not null)
 * 
 * @param msg The message to log
 * @param level The error level
 */
void do_log(const char *msg, enum log_level level);

/**
 * Log the given message (if msg is not null)
 * 
 * @param fildes The file descriptor to use
 * @param msg The message to log
 * @param level The error level
 */
void _do_log(FILE* fildes, const char* msg, enum log_level level);

/**
 * Add a target for the log action
 * 
 * @param target The path for the target
 * @param targetLevel The target level to log
 * 
 * @return 0 If everything if OK, 1 if not
 */
int add_log_target(char* target, enum log_level targetLevel);

void free_all_log_target();

#endif