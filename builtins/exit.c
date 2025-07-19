#include "builtin.h"

#include "minishell.h"
#include "builtin.h"
#include <stdlib.h>
#include <unistd.h>

static int is_numeric(const char *str)
{
    int i = 0;
    if (!str)
        return 0;
    if (str[i] == '+' || str[i] == '-')
        i++;
    while (str[i])
    {
        if (str[i] < '0' || str[i] > '9')
            return 0;
        i++;
    }
    return 1;
}

void free_env(t_env *env)
{
    t_env *tmp;

    while (env)
    {
        tmp = env->next;

        if (env->key)
            free(env->key);
        if (env->value)
            free(env->value);

        free(env);
        env = tmp;
    }
}

void exit_cleanup(t_data *data)
{
    if (data->tokens)
        free_token_list(data->tokens);
    if (data->cmds)
        free_cmd_list(data->cmds);
    if (data->env)
        free_env(data->env);
    free(data->raw_input);
}

int ft_exit(char **argv, t_data *data)
{
    long code = 0;

    write(STDOUT_FILENO, "exit\n", 5);

    if (argv[1])
    {
        if (!is_numeric(argv[1]))
        {
            ft_putstr_fd("minishell: exit: ", 2);
            ft_putstr_fd(argv[1], 2);
            ft_putstr_fd(": numeric argument required\n", 2);
            exit_cleanup(data);
            exit(255);
        }
        if (argv[2])
        {
            ft_putstr_fd("minishell: exit: too many arguments\n", 2);
            return (1);
        }
        code = ft_atoi(argv[1]);
    }
    exit_cleanup(data);
    exit((unsigned char)code);
}
