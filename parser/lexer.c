#include "minishell.h"
#include "parser.h"

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


static void	finalize_word_token(t_token **tokens,
		char **word, int *quote_type)
{
	if (*word)
	{
		add_token(tokens, *word, T_WORD, *quote_type);
		free(*word);
		*word = NULL;
	}
	*quote_type = 0;
}

static int	print_unclosed_quote(t_data *data)
{
	write(2, "minishell: syntax error: unclosed quote\n", 40);
	data->exit_status = 258;
	return (0);
}

static int	handle_quoted_section(char *input, t_data *data,
		int *i, char **word, int *q_type)
{
	char	quote_char;

	quote_char = input[*i];
	if (*q_type == 0)
		*q_type = odd_or_double_quote(quote_char);
	(*i)++;
	while (input[*i] && input[*i] != quote_char)
	{
		*word = append_char_to_str(*word, input[*i]);
		(*i)++;
	}
	if (!input[*i])
		return (print_unclosed_quote(data));
	(*i)++;
	return (1);
}

static int	append_word_char(char *input, char **word, int *i)
{
	*word = append_char_to_str(*word, input[*i]);
	(*i)++;
	return (*word != NULL);
}

static int	build_and_add_word(char *input, t_token **tokens,
		t_data *data, int *i)
{
	char	*word;
	int		quote_type;

	word = NULL;
	quote_type = 0;
	while (input[*i] && !ft_isspace(input[*i]) && !is_operator(input[*i]))
	{
		if (input[*i] == '\'' || input[*i] == '"')
		{
			if (!handle_quoted_section(input, data, i, &word, &quote_type))
			{
				if (word)
					free(word);
				return (0);
			}
		}
		else if (!append_word_char(input, &word, i))
			return (0);
	}
	finalize_word_token(tokens, &word, &quote_type);
	return (1);
}

t_token	*lexer(char *input, t_data *data)
{
	t_token	*tokens;
	int		i;
	int		step;

	tokens = NULL;
	i = 0;
	while (input[i])
	{
		if (ft_isspace(input[i]))
			i++;
		else if (is_operator(input[i]))
		{
			step = handle_redirection(input, &tokens, i, data);
			if (step == 0)
				return (free_token_list(tokens), NULL);
			i += step;
		}
		else
		{
			if (!build_and_add_word(input, &tokens, data, &i))
				return (free_token_list(tokens), NULL);
		}
	}
	return (tokens);
}
