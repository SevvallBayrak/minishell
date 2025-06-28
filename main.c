#include "minishell.h"
#include "parser.h"
#include <stdio.h>
#include <stdlib.h>
#include <readline/readline.h>
#include <readline/history.h>

void print_tokens(t_token *tokens)
{
    while (tokens)
    {
        printf("Token → Type: %d, Value: %s\n", tokens->type, tokens->value);
        tokens = tokens->next;
    }
}
int main(int argc, char **argv, char **envp)
{
	char    *line;
	t_data	data;

	(void)argc;
	(void)argv;
	(void)envp;
	data.tokens = NULL;

	while (1)
	{
		line = readline("minishell> ");
		if (!line)
		{
			printf("exit\n");
			break;
		}

		if (*line)
			add_history(line);

		// Tırnak kontrolü (syntax error)
		// if (check_unclosed_quotes(line))
		// {
		// 	printf("Syntax error: unclosed quotes\n");
		// 	free(line);
		// 	continue;
		// }

		// LEXER ÇAĞRISI
		data.tokens = lexer(line);

		// TOKENLARI GÖSTER
		printf("=== Token Listesi ===\n");
		print_tokens(data.tokens);

		// TOKENLARI TEMİZLE
		data.tokens = NULL;

		// LINE’I TEMİZLE
		free(line);
	}
	return (0);
}
