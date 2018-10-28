#include <stdlib.h>
#include <stdio.h>
#include <regex.h>

#include "../config.h"
#include "log.h"

/**
 * Checking if a regex is valid and print to stderr why the regex won't work
 * 
 * @param strRegex The regex to check
 * @return 0 if OK, 1 if not
 */
int check_regex(char *strRegex)
{
    regex_t regex;

    int regCompRes = regcomp(&regex, strRegex, 0);

    if (regCompRes != 0)
    {
        char regErrBuff[DEFAULT_BUFFER_SIZE] = "";
        char buff[DEFAULT_BUFFER_SIZE] = "";

        regerror(regCompRes, &regex, buff, DEFAULT_BUFFER_SIZE);
        sprintf(buff, "check_regex: Unable to create regex : %s", regErrBuff);

        do_log2(buff, LOG_LEVEL_ERROR);

        regfree(&regex);
        return 1;
    }
    else
    {
        regfree(&regex);
        return 0;
    }
}

/**
 * Getting a regex based on a string and handling errors
 * 
 * @param regex The regex that will be created
 * @param strRegex The original string regex
 * @param caseSensitive If the regex is case sensitive
 * @return 0 if OK, 1 if not
 */
int get_regex(regex_t *regex, const char *strRegex, const int ignoreCase)
{
    if (regex == NULL)
    {
        do_log2("get_regex: regex is NULL", LOG_LEVEL_ERROR);
        return 1;
    }

    int retCode = regcomp(regex, strRegex, REG_EXTENDED | REG_NOSUB | ((ignoreCase != 0) ? REG_ICASE : 0));

    if (retCode != 0)
    {
        char regErrBuff[DEFAULT_BUFFER_SIZE] = "";
        char buff[DEFAULT_BUFFER_SIZE] = "";

        regerror(retCode, regex, buff, DEFAULT_BUFFER_SIZE);
        sprintf(buff, "check_regex: Unable to create regex : %s", regErrBuff);

        do_log2(buff, LOG_LEVEL_ERROR);

        regfree(regex);

        return 1;
    }
    else
    {
        return 0;
    }
}