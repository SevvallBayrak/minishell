#include "parser.h"

int check_unclosed_quotes(const char *str)
{
    char flag = 0;
    while (*str)
    {
        if ((*str == '\'' || *str == '"') && flag == 0)
            flag = *str;
        else if (*str == flag)
            flag = 0;
        str++;
    }
    return (flag != 0);
}
