#include "builtin.h"

int ft_env(t_data *data)
{
    t_env *env = data->env;

    while (env)
    {
        if (env->value)
        {
            write(1, env->key, ft_strlen(env->key));
            write(1, "=", 1);
            write(1, env->value, ft_strlen(env->value));
            write(1, "\n", 1);
        }
        env = env->next;
    }
    return 0;
}