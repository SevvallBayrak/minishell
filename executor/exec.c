#include "minishell.h"
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <stdio.h>

typedef enum e_path_result
{
    PATH_OK = 0,
    PATH_NOT_FOUND = 127,
    PATH_NO_PERMISSION = 126
}   t_path_result;

static char *search_path_dirs(char *path_env, char *cmd, int *result)
{
    int i = 0, j;
    char *dir;
    char *full_path;

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

        if (access(full_path, F_OK) == 0)
        {
            if (access(full_path, X_OK) == 0)
            {
                *result = PATH_OK;
                return full_path;
            }
            *result = PATH_NO_PERMISSION;
            free(full_path);
            return NULL;
        }
        free(full_path);
        i = j + 1;
    }
    *result = PATH_NOT_FOUND;
    return NULL;
}

char *get_command_path(char *cmd, t_data *data, int *result)
{
    char *path_env;

    *result = PATH_NOT_FOUND;

    if (!cmd)
        return NULL;

    if (cmd[0] == '/' || (cmd[0] == '.' && cmd[1] == '/'))
    {
        if (access(cmd, F_OK) == 0)
        {
            if (access(cmd, X_OK) == 0)
            {
                *result = PATH_OK;
                return ft_strdup(cmd);
            }
            *result = PATH_NO_PERMISSION;
            return NULL;
        }
        return NULL;
    }

    path_env = get_env_value(data->env, "PATH");
    if (!path_env)
        return NULL;

    return search_path_dirs(path_env, cmd, result);
}

void run_child_process(char **argv, char **envp, t_data *data)
{
    int path_result;
    char *path = get_command_path(argv[0], data, &path_result);

    if (!path)
    {
        if (path_result == PATH_NO_PERMISSION)
            write(2, "Permission denied\n", 18);
        else
            write(2, "Command not found\n", 18);
        free_argv(envp);
        exit(path_result == PATH_NO_PERMISSION ? 126 : 127);
    }

    // execve başarısız olursa buraya düşecek
    execve(path, argv, envp);

    // Eğer execve başarısızsa, büyük ihtimalle izin veya dizin hatası
    write(2, "Permission denied\n", 18);
    free(path);
    free_argv(envp);
    exit(126);
}


int execute_command(char **argv, t_data *data)
{
    pid_t pid;
    int status;
    char **envp = env_to_envp(data->env);

    if (!envp)
        return (1);

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
    return 0;
}
