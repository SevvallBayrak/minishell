/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sbayrak <sbayrak@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/25 05:14:01 by sbayrak           #+#    #+#             */
/*   Updated: 2025/07/27 06:45:17 by sbayrak          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	is_numeric(const char *str)
{
	int	i;

	i = 0;
	if (!str)
		return (0);
	if (str[i] == '+' || str[i] == '-')
		i++;
	while (str[i])
	{
		if (str[i] < '0' || str[i] > '9')
			return (0);
		i++;
	}
	return (1);
}

// free_utils.c veya env_utils.c (free_env fonksiyonu)
void free_env(t_env *env_list)
{
    t_env *current = env_list;
    t_env *next_node;

    while (current)
    {
        next_node = current->next;
        if (current->key)
            free(current->key); // Anahtarı serbest bırak
        if (current->value)
            free(current->value); // Değeri serbest bırak
        free(current); // Düğümün kendisini serbest bırak
        current = next_node;
    }
}

void	exit_cleanup(t_data *data)
{
	if (data->tokens)
		free_token_list(data->tokens);
	if (data->cmds)
		free_cmd_list(data->cmds);
	if (data->env)
		free_env(data->env);
	if (data->raw_input)
		free(data->raw_input);
}

int	ft_exit(char **argv, t_data *data)
{
	long	code;

	code = 0;
	write(STDOUT_FILENO, "exit\n", 5);
	if (argv[1])
	{
		if (!is_numeric(argv[1]))
		{
			ft_putstr_fd("minishell: exit: ", 2);
			ft_putstr_fd(argv[1], 2);
			ft_putstr_fd(": numeric argument required\n", 2);
			exit_cleanup(data);
			exit(2);
		}
		if (argv[2])
		{
			ft_putstr_fd("minishell: exit: too many arguments\n", 2);
			return (1);
		}
		code = ft_atoi(argv[1]);
	}
	exit_cleanup(data);
	exit((unsigned char)code);
}
