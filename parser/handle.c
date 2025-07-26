/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sbayrak <sbayrak@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/25 06:07:19 by sbayrak           #+#    #+#             */
/*   Updated: 2025/07/26 23:04:28 by sbayrak          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	handle_word(char *input, t_token **tokens, int i)
{
	int		start;
	char	*word;

	start = i;
	while (input[i] && !is_special_char(input[i]))
		i++;
	if (i > start)
	{
		word = ft_strndup(&input[start], i - start);
		add_token(tokens, word, T_WORD, 0);
		free(word);
	}
	return (i - start);
}

static int	handle_quote_error(t_data *data)
{
	write(2, "minishell: syntax error: unclosed quote\n", 40);
	data->exit_status = 258;
	return (0);
}

int	handle_quote(char *input, t_token **tokens, int i, t_data *data)
{
	int		start;
	char	quote;
	char	*word;

	quote = input[i];
	start = i + 1;
	i = start;
	while (input[i] && input[i] != quote)
		i++;
	if (!input[i])
		return (handle_quote_error(data));
	word = ft_strndup(&input[start], i - start);
	if (!word)
		return (0);
	add_token(tokens, word, T_WORD, odd_or_double_quote(quote));
	free(word);
	return (i - start + 2);
}

int	hanldle_append_heredoc(char *input, t_token **tokens, int i)
{
	if (input[i] == '>')
	{
		add_token(tokens, ">", T_REDIR_OUT, 0);
		return (1);
	}
	if (input[i] == '<')
	{
		add_token(tokens, "<", T_REDIR_IN, 0);
		return (1);
	}
	return (0);
}

int	handle_redirection(char *input, t_token **tokens, int i, t_data *data)
{
	if (is_invalid_redir(input, i, data))
		return (0);
	if (input[i + 1])
	{
		if (input[i] == '>' && input[i + 1] == '>')
		{
			add_token(tokens, ">>", T_REDIR_APPEND, 0);
			return (2);
		}
		if (input[i] == '<' && input[i + 1] == '<')
		{
			add_token(tokens, "<<", T_HEREDOC, 0);
			return (2);
		}
	}
	if (input[i] == '|')
	{
		add_token(tokens, "|", T_PIPE, 0);
		return (1);
	}
	return (hanldle_append_heredoc(input, tokens, i));
}
