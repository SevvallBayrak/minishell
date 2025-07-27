/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sbayrak <sbayrak@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/25 06:58:10 by sbayrak           #+#    #+#             */
/*   Updated: 2025/07/27 22:23:25 by sbayrak          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*get_env_value(t_env *env_list, const char *key)
{
	while (env_list)
	{
		if (ft_strncmp(env_list->key, key, (ft_strlen(key) + 1)) == 0)
		{
			if (env_list->value)
				return (env_list->value);
			return (NULL);
		}
		env_list = env_list->next;
	}
	return (NULL);
}
void	update_env_var(t_data *data, const char *key, const char *value)
{
	t_env	*env;
	t_env	*new_node;

	env = data->env;
	while (env)
	{
		if (ft_strncmp(env->key, key, (ft_strlen(key) + 1)) == 0)
		{
			free(env->value);
			env->value = ft_strdup(value);
			env->is_export_only = 0; // Artık env değişkeni oldu
			return ;
		}
		env = env->next;
	}
	new_node = malloc(sizeof(t_env));
	if (!new_node)
		return ;
	new_node->key = ft_strdup(key);
	new_node->value = ft_strdup(value);
	new_node->is_export_only = 0;
	new_node->next = NULL;
	env_add_back(&data->env, new_node);
}

static int	count_env_vars(t_env *env)
{
	int	count;

	count = 0;
	while (env)
	{
		if (env->value)
			count++;
		env = env->next;
	}
	return (count);
}

static char	*join_env_entry(const char *key, const char *value)
{
	char	*res;
	size_t	len;

	len = ft_strlen(key) + ft_strlen(value) + 2;
	res = malloc(len);
	if (!res)
		return (NULL);
	ft_strlcpy(res, key, len);
	ft_strlcat(res, "=", len);
	ft_strlcat(res, value, len);
	return (res);
}

char	**env_to_envp(t_env *env)
{
	int		i;
	char	**envp;

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

void	add_exported_var(t_data *data, const char *key)
{
	t_env	*new_node;

	if (find_exported_var(data->exported_vars, key))
		return ;
	new_node = malloc(sizeof(t_env));
	if (!new_node)
		return ;
	new_node->key = ft_strdup(key);
	new_node->value = NULL;
	new_node->next = NULL;
	env_add_back(&data->exported_vars, new_node);
}

t_env	*find_exported_var(t_env *exported_vars, const char *key)
{
	while (exported_vars)
	{
		if (ft_strncmp(exported_vars->key, key, (ft_strlen(key) + 1)) == 0)
			return (exported_vars);
		exported_vars = exported_vars->next;
	}
	return (NULL);
}
