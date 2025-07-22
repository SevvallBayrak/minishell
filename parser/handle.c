#include "minishell.h"
#include "utils.h"
#include "parser.h"

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
	start = ++i;
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

int	is_invalid_redir(const char *input, int i, t_data *data)
{
	if (input[i] == '<' && input[i + 1] == '<' && input[i + 2] == '<')
	{
		write(2, "minishell: syntax error\n", 25);
		data->exit_status = 258;
		return (1);
	}
	if (input[i] == '>' && input[i + 1] == '>' && input[i + 2] == '>')
	{
		write(2, "minishell: syntax error\n", 25);
		data->exit_status = 258;
		return (1);
	}
	return (0);
}

int	handle_redirection(char *input, t_token **tokens, int i, t_data *data)
{
	if (is_invalid_redir(input, i, data))
		return (0);
	if (input[i] == '>' && input[i + 1] == '>')
		return (add_token(tokens, ">>", T_REDIR_APPEND, 0), 2);
	if (input[i] == '<' && input[i + 1] == '<')
		return (add_token(tokens, "<<", T_HEREDOC, 0), 2);
	if (input[i] == '>')
		return (add_token(tokens, ">", T_REDIR_OUT, 0), 1);
	if (input[i] == '<')
		return (add_token(tokens, "<", T_REDIR_IN, 0), 1);
	if (input[i] == '|')
		return (add_token(tokens, "|", T_PIPE, 0), 1);
	return (0);
}
