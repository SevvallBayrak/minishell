/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sbayrak <sbayrak@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/25 05:09:56 by sbayrak           #+#    #+#             */
/*   Updated: 2025/07/28 15:50:38 by sbayrak          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	print_env_variable(t_env *env)
{
	if (write(1, env->key, ft_strlen(env->key)) < 0)
		return (1);
	if (write(1, "=", 1) < 0)
		return (1);
	if (write(1, env->value, ft_strlen(env->value)) < 0)
		return (1);
	if (write(1, "\n", 1) < 0)
		return (1);
	return (0);
}

int	ft_env(t_data *data)
{
	t_env	*env;
	char	*path_value;

	path_value = get_env_value(data->env, "PATH");
	if (!path_value)
	{
		ft_putstr_fd("env: No such file or directory\n", 2);
		return (127);
	}
	env = data->env;
	while (env)
	{
		if (env->value)
		{
			if (print_env_variable(env))
				return (1);
		}
		env = env->next;
	}
	return (0);
}
