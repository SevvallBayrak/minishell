#include <minishell.h>
#include <errno.h>

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

static char *search_path_dirs(char *path_env, char *cmd)
{
    int i = 0, j;
    char *dir, *full_path;

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
char *get_command_path(char *cmd, t_data *data)
{
    char *path_env;

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
    return search_path_dirs(path_env, cmd);
}

void run_child_process(char **argv, char **envp, t_data *data)
{
    char *path = get_command_path(argv[0], data);

    if (!path)
    {
        write(2, "Command not found\n", 18);
        free_argv(envp);
        exit(127); // Komut bulunamadı
    }

    // İzin kontrolü
    if (access(path, X_OK) != 0)
    {
        write(2, "Permission denied\n", 18);
        free(path);
        free_argv(envp);
        exit(126); // Çalıştırma izni yok
    }

    execve(path, argv, envp);

    // execve başarısızsa
    perror("execve");
    free(path);
    free_argv(envp);
    exit(1); // Diğer hata
}


int execute_command(char **argv, t_data *data)
{
    pid_t pid;
    int status;
    char **envp = env_to_envp(data->env);

    if (!envp)
        return (1); // Bellek hatası

    pid = fork();
    if (pid == 0)
        run_child_process(argv, envp, data);
    else if (pid < 0)
    {
        perror("fork");
        free_argv(envp);
        return 1;
    }
    waitpid(pid, &status, 0);
    free_argv(envp);
    if (WIFEXITED(status))
        return WEXITSTATUS(status);
    if (WIFSIGNALED(status))
        return 128 + WTERMSIG(status);
    return 1;
}



