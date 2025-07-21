#include "builtin.h"

int ft_echo(char **argv)
{
    int i = 1;
    int newline = 1;

    if (argv[i] && ft_strncmp(argv[i], "-n", 3) == 0)
    {
        newline = 0;
        i++;
    }
    while (argv[i])
    {
		printf("%s", argv[i]);
		if (argv[i + 1])
			printf(" ");
		i++;
    }
    if (newline)
        printf("\n");
    return (0);
}
