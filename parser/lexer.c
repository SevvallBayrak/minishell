#include "minishell.h"
#include "utils.h"
#include "parser.h"

int dispatch_lexer(char *input, int i, t_token **tokens)
{
	int step;

	if (input[i] == '\'' || input[i] == '"')
	{
		step = handle_quote(input, tokens, i);
		if (step == 0)
			return (-1); // hata
	}
	else if (is_operator(input[i]))
	{
        step = handle_redirection(input, tokens, i);
        if (step == 0)
			return (-1);
    }
	else
		step = handle_word(input, tokens, i);

	return (step);
}
t_token *lexer(char *input)
{
	t_token *tokens = NULL;
	int i = 0;
	int step;

	while (input[i])
	{
		if (ft_isspace(input[i]))
			i++;
		else
		{
			step = dispatch_lexer(input, i, &tokens);
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
