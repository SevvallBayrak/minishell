/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sbayrak <sbayrak@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/28 19:50:48 by sbayrak           #+#    #+#             */
/*   Updated: 2025/07/28 21:59:58 by sbayrak          ###   ########.fr       */
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

int	redir_no_arg(t_cmd *cmd, t_data *data)
{
	if (cmd->infile)
	{
		data->exit_status = 1;
		return (1);
	}
	if (cmd->outfile)
	{
		if (redirect_in(cmd, data) || redirect_out(cmd, data))
			return (1);
		data->exit_status = 0;
	}
	return (0);
}
