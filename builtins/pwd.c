
#include "builtin.h"

int ft_pwd(t_data *data)
{
    char cwd[1024];

    (void)data;

    if (!getcwd(cwd, sizeof(cwd)))
    {
        perror("pwd");
        return 1;
    }

    write(1, cwd, ft_strlen(cwd));
    write(1, "\n", 1);
    return 0;
}
