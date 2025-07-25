/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sbayrak <sbayrak@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/25 05:19:03 by sbayrak           #+#    #+#             */
/*   Updated: 2025/07/25 06:57:59 by sbayrak          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_unset(char **argv, t_data *data)
{
	int	i;
	int	has_error;

	i = 1;
	has_error = 0;
	while (argv[i])
	{
		if (!is_valid_key(argv[i]))
		{
			write(2, "unset: not a valid identifier\n", 31);
			has_error = 1;
		}
		else
			remove_env_var(&data->env, argv[i]);
		i++;
	}
	return (has_error);
}
