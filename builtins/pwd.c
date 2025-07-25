/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sbayrak <sbayrak@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/25 05:17:42 by sbayrak           #+#    #+#             */
/*   Updated: 2025/07/25 06:57:55 by sbayrak          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_pwd(t_data *data)
{
	char	cwd[1024];

	(void)data;
	if (!getcwd(cwd, sizeof(cwd)))
	{
		perror("pwd");
		return (1);
	}
	if (write(1, cwd, strlen(cwd)) == -1
		|| write(1, "\n", 1) == -1)
	{
		perror("pwd: write error");
		return (1);
	}
	return (0);
}
