
#include "minishell.h"

char *get_env_value(t_env *env, const char *key)
{
    while (env)
    {
        if (strcmp(env->key, key) == 0)
            return env->value;
        env = env->next;
    }
    return NULL;
}

void update_env_var(t_data *data, const char *key, const char *value)
{
    t_env *env = data->env;

    while (env)
    {
        if (strcmp(env->key, key) == 0)
        {
            free(env->value);
            env->value = ft_strdup(value);
            return;
        }
        env = env->next;
    }
    t_env *new_node = malloc(sizeof(t_env));
    if (!new_node)
        return;
    new_node->key = ft_strdup(key);
    new_node->value = ft_strdup(value);
    new_node->next = NULL;
    env_add_back(&data->env, new_node);
}

void remove_env_var(t_env **env, const char *key)
{
    t_env *current = *env;
    t_env *prev = NULL;

    while (current)
    {
        if (strcmp(current->key, key) == 0)
        {
            if (prev)
                prev->next = current->next;
            else
                *env = current->next;

            free(current->key);
            free(current->value);
            free(current);
            return;
        }
        prev = current;
        current = current->next;
    }
}

static int count_env_vars(t_env *env)
{
    int count = 0;

    while (env)
    {
        if (env->value)
            count++;
        env = env->next;
    }
    return (count);
}

static char *join_env_entry(const char *key, const char *value)
{
    char    *res;
    size_t  len;

    len = ft_strlen(key) + ft_strlen(value) + 2; // '=' ve '\0' için +2
    res = malloc(len);
    if (!res)
        return (NULL);

    // önce key'i kopyala
    ft_strlcpy(res, key, len);

    // sonuna '=' ekle
    ft_strlcat(res, "=", len);

    // sonuna value ekle
    ft_strlcat(res, value, len);

    return (res);
}


char **env_to_envp(t_env *env)
{
    int     i;
    char    **envp;

    envp = malloc(sizeof(char *) * (count_env_vars(env) + 1));
    if (!envp)
        return (NULL);
    i = 0;
    while (env)
    {
        if (env->value)
        {
            envp[i] = join_env_entry(env->key, env->value);
            if (!envp[i])
                return (NULL);
            i++;
        }
        env = env->next;
    }
    envp[i] = NULL;
    return (envp);
}
