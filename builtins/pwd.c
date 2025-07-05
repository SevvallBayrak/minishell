
#include "builtin.h"

int ft_pwd(t_data *data)
{
    char *pwd = get_env_value(data->env, "PWD");

    if (!pwd)
    {
        write(2, "pwd: PWD not set\n", 18);
        return 1;
    }
    write(1, pwd, ft_strlen(pwd));
    write(1, "\n", 1);
    return 0;
}

