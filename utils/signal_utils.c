/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sbayrak <sbayrak@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/28 19:35:14 by sbayrak           #+#    #+#             */
/*   Updated: 2025/07/28 20:05:38 by sbayrak          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	check_signal_exit_status(t_data *data)
{
	if (g_sigquit_flag == 130)
	{
		data->exit_status = 130;
		g_sigquit_flag = 0;
	}
	else if (g_sigquit_flag == 131)
	{
		data->exit_status = 131;
		g_sigquit_flag = 0;
	}
}

void	exit_and_free(t_data *data)
{
	rl_clear_history();
	if (data->env)
		free_env(data->env);
	if (data->exported_vars)
		free_env(data->exported_vars);
	exit(131);
}

void	data_node_null_and_init_sigenv(int argc, char **argv,
						char **envp, t_data *data)
{
	init_data_node(argc, argv, data);
	init_signal();
	init_env(data, envp);
}

void	init_data_node(int argc, char **argv, t_data *data)
{
	(void)argc;
	(void)argv;
	data->tokens = NULL;
	data->env = NULL;
	data->exit_status = 0;
	data->raw_input = NULL;
}

void	print_export_list(t_env *env)
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
