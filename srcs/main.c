#include "minishell.h"

int	main(void)
{
	char	*input;

	while (1)
	{
		input = readline("minishell> ");
		if (!input) // Ctrl+D
			break;
		if (*input)
			add_history(input);
		free(input);
	}
	printf("exit\n");
	return (0);
}
