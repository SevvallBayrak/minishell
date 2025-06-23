#include "minishell.h"
#include "parser.h"

int	main(void)
{
	char	*input;
	char	**tokens;
	int		i;

	while (1)
	{
		input = readline("minishell> ");
		if (!input)
			break;
		if (*input)
			add_history(input);
		// if (check_unclosed_quotes(input))
		// {
		// 	printf("Syntax error: unclosed quote detected.\n");
		// 	free(input);
		// 	continue;
		// }

		tokens = tokenize_input(input);
		for (i = 0; tokens[i]; i++)
			printf("Token[%d]: %s\n", i, tokens[i]);

		for (i = 0; tokens[i]; i++)
			free(tokens[i]);
		free(tokens);
		free(input);
	}
	printf("exit\n");
	return (0);
}