#include <stdlib.h>
#include <stdio.h>
#include <regex.h>

int check_regex(char *strRegex)
{
    regex_t regex;

    int regCompRes = regcomp(&regex, strRegex, 0);

    if (regCompRes != 0)
    {
        char buff[200];
        regerror(regCompRes, &regex, buff, 200);
        printf("check_regex: Unable to create regex : %s", buff);
        regfree(&regex);
        return 1;
    }
    else
    {
        regfree(&regex);
        return 0;
    }
}

int get_regex(regex_t* regex, char* strRegex, int ignoreCase)
{
    if(regex == NULL)
    {
        printf ("get_regex: regex is NULL");
        return 1;
    }

    int retCode = regcomp(regex, strRegex, REG_EXTENDED | REG_NOSUB | ((ignoreCase != 0) ? REG_ICASE : 0));

    if (retCode != 0)
    {
        char buff[200];
        regerror(retCode, regex, buff, 200);
        printf("check_regex: Unable to create regex : %s\n", buff);
        regfree(regex);
        return 2;
    }
    else
    {
        return 0;
    }
}