#include "builtin.h"

int is_builtin(const char *cmd)
{
    if (!cmd)
        return(0);
    return(
        ft_strncmp(cmd, "echo", 5) == 0 ||
        ft_strncmp(cmd, "cd", 3) == 0 ||
        ft_strncmp(cmd, "pwd", 4) == 0 ||
        ft_strncmp(cmd, "env", 4) == 0 ||
        ft_strncmp(cmd, "export", 7) == 0 ||
        ft_strncmp(cmd, "unset", 6) == 0 ||
        ft_strncmp(cmd, "exit", 5) == 0
    );
}
int exec_builtin(t_cmd *cmd, t_data *data)
{
    if (!cmd || !cmd->argv || !cmd->argv[0])
        return (1);

    if (ft_strncmp(cmd->argv[0], "echo", 5) == 0)
        return ft_echo(cmd->argv);
    else if (ft_strncmp(cmd->argv[0], "cd", 3) == 0)
        return ft_cd(cmd->argv, data);
    else if (ft_strncmp(cmd->argv[0], "pwd", 4) == 0)
        return ft_pwd(data);
    else if (ft_strncmp(cmd->argv[0], "env", 4) == 0)
        return ft_env(data);
    else if (ft_strncmp(cmd->argv[0], "export", 7) == 0)
        return ft_export(cmd->argv, data);
    else if (ft_strncmp(cmd->argv[0], "unset", 6) == 0)
        return ft_unset(cmd->argv, data);
    else if (ft_strncmp(cmd->argv[0], "exit", 5) == 0)
        return ft_exit(cmd->argv, data);
    return (1);
}
