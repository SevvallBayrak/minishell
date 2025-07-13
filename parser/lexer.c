#include "minishell.h"
#include "utils.h"
#include "parser.h"
#include <unistd.h> // write için

int dispatch_lexer(char *input, int i, t_token **tokens, t_data *data)
{
	int step = 0;

	if (input[i] == '\'' || input[i] == '"')
	{
		step = handle_quote(input, tokens, i, data);
		if (step == 0)
			return (-1); // hata
	}
	else if (input[i] == '|')
	{
		add_token(tokens, "|", T_PIPE, 0);
		step += 1;
	}
	else if (is_operator(input[i]))
	{
		step = handle_redirection(input, tokens, i, data);
		if (step == 0)
			return (-1);
	}
	else
		step = handle_word(input, tokens, i);

	return (step);
}

t_token *lexer(char *input, t_data *data)
{
	t_token *tokens = NULL;
	int i = 0;
	int step = 0;

	while (input[i])
	{
		if (ft_isspace(input[i]))
			i++;
		else
		{
			step = dispatch_lexer(input, i, &tokens, data);
			if (step == -1)
			{
				free_token_list(tokens);
				return (NULL);
			}
			i += step;
		}
	}
	return tokens;
}
