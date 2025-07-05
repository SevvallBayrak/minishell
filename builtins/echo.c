#include "builtin.h"

int ft_echo(char **argv)
{
    int i;
    int newline;

    newline = 1;
    i = 1;
    if (argv[i] && ft_strncmp(argv[i], "-n", 3) == 0)
    {
        newline = 0;
        i++;
    }
    while(argv[i])
    {
        write(1, argv[i], strlen(argv[i]));
        if (argv[i + 1])
            write(1, " ", 1);
        i++;
    }
    if (newline)
        write(1, "\n", 1);
    return (0);
}
