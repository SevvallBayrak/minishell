#include <minishell.h>

int executor_execute(t_cmd *cmds, t_data *data)
{
    while (cmds)
    {
        if (is_builtin(cmds->argv[0]))
            exec_builtin(cmds, data);
        else
            execute_command(cmds->argv, data);
        cmds = cmds->next;
    }
    return (0);
}

char *get_command_path(char *cmd, t_data *data)
{
    int     i = 0, j = 0;
    char    *path_env;
    char    *dir;
    char    *full_path;

    if (!cmd)
        return (NULL);
    if (cmd[0] == '/' || (cmd[0] == '.' && cmd[1] == '/'))
    {
        if (access(cmd, X_OK) == 0)
            return (ft_strdup(cmd));
        return (NULL);
    }
    path_env = get_env_value(data->env, "PATH");
    if (!path_env)
        return (NULL);
    while (path_env[i])
    {
        while (path_env[i] == ':')
            i++;
        j = i;
        while (path_env[j] && path_env[j] != ':')
            j++;
        dir = ft_substr(path_env, i, j - i);
        full_path = ft_strjoin_path(dir, cmd);
        free(dir);
        if (access(full_path, X_OK) == 0)
            return (full_path);
        free(full_path);
        i = j + 1;
    }
    return (NULL);
}

void execute_command(char **argv, t_data *data)
{
    pid_t   pid;
    char    *path;
    char    **envp;

    envp = env_to_envp(data->env);
    if (!envp)
        return ;
    pid = fork();
    if (pid == 0)
    {
        path = get_command_path(argv[0], data); // burada artık data alıyor
        if (!path)
        {
            write(2, "Command not found\n", 18);
            free_argv(envp);
            exit(127);
        }
        execve(path, argv, envp);
        perror("execve");
        free(path);
        free_argv(envp);
        exit(1);
    }
    else if (pid > 0)
        waitpid(pid, NULL, 0);
    else
        perror("fork");
    free_argv(envp);
}
