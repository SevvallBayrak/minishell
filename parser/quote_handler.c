/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quote_handler.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sbayrak <sbayrak@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/25 06:37:51 by sbayrak           #+#    #+#             */
/*   Updated: 2025/07/28 22:02:17 by sbayrak          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	check_unclosed_quotes(const char *str)
{
	char	flag;

	flag = 0;
	while (*str)
	{
		if ((*str == '\'' || *str == '"') && flag == 0)
			flag = *str;
		else if (*str == flag)
			flag = 0;
		str++;
	}
	return (flag != 0);
}

int	validate_syntax(t_token *tok, t_data *data)
{
	if (!tok)
		return (1);
	while (tok)
	{
		if (tok->type == T_PIPE
			&& (!tok->next || tok->next->type == T_PIPE))
		{
			write(2, "minishell: syntax error near unexpected token `|'\n", 50);
			data->exit_status = 2;
			return (0);
		}
		if (tok->type >= T_REDIR_IN && tok->type <= T_HEREDOC)
		{
			if (!tok->next || tok->next->type != T_WORD)
			{
				write(2,
					"minishell: syntax error near unexpected token `newline'\n",
					57);
				data->exit_status = 2;
				return (0);
			}
		}
		tok = tok->next;
	}
	return (1);
}

int	odd_or_double_quote(char quote)
{
	if (quote == '\'')
		return (1);
	else if (quote == '"')
		return (2);
	return (0);
}
