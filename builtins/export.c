/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sbayrak <sbayrak@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/25 05:16:51 by sbayrak           #+#    #+#             */
/*   Updated: 2025/07/28 20:05:23 by sbayrak          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_export(char **argv, t_data *data)
{
	int	i;
	int	has_error;

	i = 1;
	has_error = 0;
	if (!argv[1])
	{
		print_export_list(data->env);
		return (0);
	}
	while (argv[i])
	{
		if (!process_export_arg(argv[i], data))
			has_error = 1;
		i++;
	}
	return (has_error);
}

static t_env	*create_new_env_node(char *key, char *value)
{
	t_env	*new_node;

	new_node = malloc(sizeof(t_env));
	if (!new_node)
		return (NULL);
	new_node->key = ft_strdup(key);
	if (value)
	{
		new_node->value = ft_strdup(value);
	}
	else
	{
		new_node->value = NULL;
	}
	new_node->next = NULL;
	return (new_node);
}

static int	update_existing_env(t_env *env, char *key, char *value)
{
	while (env)
	{
		if (ft_strncmp(env->key, key, (ft_strlen(key) + 1)) == 0)
		{
			if (value)
			{
				free(env->value);
				env->value = ft_strdup(value);
			}
			return (1);
		}
		env = env->next;
	}
	return (0);
}

static void	split_key_value(char *arg, char **key, char **value)
{
	char	*equal_sign;

	equal_sign = ft_strchr(arg, '=');
	if (equal_sign)
	{
		*equal_sign = '\0';
		*key = arg;
		*value = equal_sign + 1;
	}
	else
	{
		*key = arg;
		*value = NULL;
	}
}

int	process_export_arg(char *arg, t_data *data)
{
	char	*key;
	char	*value;
	t_env	*new_node;

	split_key_value(arg, &key, &value);
	if (!is_valid_key(key))
	{
		write(2, "export: not a valid identifier\n", 31);
		return (0);
	}
	if (update_existing_env(data->env, key, value))
		return (1);
	new_node = create_new_env_node(key, value);
	if (!new_node)
		return (0);
	env_add_back(&data->env, new_node);
	return (1);
}
