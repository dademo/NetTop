#ifndef TOOLS_H_INCLUDED
#define TOOLS_H_INCLUDED

/**
 * Checking if a regex is valid and print to stderr why the regex won't work
 * 
 * @param strRegex The regex to check
 * @return 0 if OK, 1 if not
 */
int check_regex(char* strRegex);

int get_regex(regex_t* regex, char* strRegex, int caseSensitive);

#endif