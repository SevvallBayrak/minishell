#include <minishell.h>

int executor_execute(t_cmd *cmds, t_data *data)
{
    while (cmds)
    {
        if (is_builtin(cmds->argv[0]))
            data->exit_status = exec_builtin(cmds, data);
        else
            data->exit_status = execute_command(cmds->argv, data);
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

int execute_command(char **argv, t_data *data)
{
    pid_t   pid;
    char    *path;
    char    **envp;
    int     status;

    envp = env_to_envp(data->env);
    if (!envp)
        return (1); // Bellek hatası gibi genel hata
    pid = fork();
    if (pid == 0)
    {
        path = get_command_path(argv[0], data); // Komutun tam yolu alınır
        if (!path)
        {
            write(2, "Command not found\n", 18);
            free_argv(envp);
            exit(127); // Bash uyumlu hata kodu
        }
        execve(path, argv, envp);
        perror("execve");
        free(path);
        free_argv(envp);
        exit(1); // execve başarısız olursa
    }
    else if (pid > 0)
    {
        waitpid(pid, &status, 0);
        free_argv(envp);

        if (WIFEXITED(status))
            return (WEXITSTATUS(status)); // çocuk başarıyla çıktıysa onun return değeri
        else if (WIFSIGNALED(status))
            return (128 + WTERMSIG(status)); // Ctrl+C gibi sinyalle çıkış
        else
            return (1); // Diğer durumlar
    }
    else
    {
        perror("fork");
        free_argv(envp);
        return (1); // fork başarısızsa
    }
}


