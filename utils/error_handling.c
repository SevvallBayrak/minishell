/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error_handling.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sbayrak <sbayrak@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/25 06:49:31 by sbayrak           #+#    #+#             */
/*   Updated: 2025/07/25 06:49:33 by sbayrak          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*append_char_to_str(char *s, char c)
{
	char	*new_s;
	size_t	len;

	if (!s)
	{
		new_s = malloc(sizeof(char) * 2);
		if (!new_s)
			return (NULL);
		new_s[0] = c;
		new_s[1] = '\0';
		return (new_s);
	}
	len = ft_strlen(s);
	new_s = malloc(sizeof(char) * (len + 2));
	if (!new_s)
		return (NULL);
	ft_strlcpy(new_s, s, len + 1);
	new_s[len] = c;
	new_s[len + 1] = '\0';
	free(s);
	return (new_s);
}

int	print_unclosed_quote(t_data *data)
{
	write(2, "minishell: syntax error: unclosed quote\n", 40);
	data->exit_status = 258;
	return (0);
}
