/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sbayrak <sbayrak@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/25 05:16:51 by sbayrak           #+#    #+#             */
/*   Updated: 2025/07/25 06:57:51 by sbayrak          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_valid_key(const char *key)
{
	int	i;

	i = 0;
	if (!key || (!ft_isalpha(key[0]) && key[0] != '_'))
		return (0);
	while (key[i])
	{
		if (!ft_isalnum(key[i]) && key[i] != '_')
			return (0);
		i++;
	}
	return (1);
}

static void	print_export_list(t_env *env)
{
	t_env	*current;

	current = env;
	while (current)
	{
		printf("declare -x %s", current->key);
		if (current->value)
			printf("=\"%s\"", current->value);
		printf("\n");
		current = current->next;
	}
}

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

int	process_export_arg(char *arg, t_data *data)
{
	char	*equal_sign;
	char	*key;
	char	*value;

	equal_sign = ft_strchr(arg, '=');
	if (equal_sign)
	{
		*equal_sign = '\0';
		key = arg;
		value = equal_sign + 1;
	}
	else
	{
		key = arg;
		value = NULL;
	}
	if (!is_valid_key(key))
	{
		write(2, "export: not a valid identifier\n", 31);
		return (0);
	}
	if (key && value)
		update_env_var(data, key, value);
	return (1);
}
