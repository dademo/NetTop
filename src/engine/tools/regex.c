#include <stdlib.h>
#include <stdio.h>
#include <regex.h>

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
        char buff[200];
        regerror(regCompRes, &regex, buff, 200);
        fprintf(stderr, "check_regex: Unable to create regex : %s\n", buff);
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
int get_regex(regex_t* regex, const char* strRegex, const int ignoreCase)
{
    if(regex == NULL)
    {
        fprintf(stderr, "get_regex: regex is NULL\n");
        return 1;
    }

    int retCode = regcomp(regex, strRegex, REG_EXTENDED | REG_NOSUB | ((ignoreCase != 0) ? REG_ICASE : 0));

    if (retCode != 0)
    {
        char buff[200];
        regerror(retCode, regex, buff, 200);
        fprintf(stderr, "check_regex: Unable to create regex : %s\n", buff);
        regfree(regex);
        return 2;
    }
    else
    {
        return 0;
    }
}