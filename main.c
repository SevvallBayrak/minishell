#include "minishell.h"
#include "parser.h"
#include <stdio.h>
#include <stdlib.h>
#include <readline/readline.h>
#include <readline/history.h>

#include <stdlib.h>

void free_argv(char **argv)
{
    int i = 0;
    if (!argv)
        return;
    while (argv[i])
    {
        free(argv[i]);
        i++;
    }
    free(argv);
}

void free_cmd_list(t_cmd *cmd)
{
    t_cmd *tmp;

    while (cmd)
    {
        tmp = cmd->next;

        // argv dizisini serbest bırak
        free_argv(cmd->argv);

        // infile, outfile ve heredoc_delim varsa serbest bırak
        if (cmd->infile)
            free(cmd->infile);
        if (cmd->outfile)
            free(cmd->outfile);
        if (cmd->heredoc_delim)
            free(cmd->heredoc_delim);

        // cmd yapısını serbest bırak
        free(cmd);

        cmd = tmp;
    }
}

void	print_cmd_list(t_cmd *cmds)
{
	int i;
	int index = 1;

	while (cmds)
	{
		printf("🔹 Command Block %d:\n", index++);

		// Argüman listesi (argv)
		i = 0;
		if (cmds->argv)
		{
			printf("  argv: ");
			while (cmds->argv[i])
				printf("[%s] ", cmds->argv[i++]);
			printf("\n");
		}
		else
			printf("  argv: (null)\n");

		// Redirection'lar
		if (cmds->infile)
			printf("  infile: %s\n", cmds->infile);
		if (cmds->outfile)
			printf("  outfile: %s (append=%d)\n", cmds->outfile, cmds->append);
		if (cmds->is_heredoc)
			printf("  heredoc_delim: %s\n", cmds->heredoc_delim);

		// Sonraki komuta geç
		cmds = cmds->next;
		printf("\n");
	}
}

void print_tokens(t_token *tokens)
{
    while (tokens)
    {
        printf("Token → Type: %d, Value: %s\n", tokens->type, tokens->value);
        tokens = tokens->next;
    }
}

int	main(int argc, char **argv, char **envp)
{
	char	*line;
	t_data	data;
	t_cmd	*cmds;

	(void)argc;
	(void)argv;
	data.tokens = NULL;
	data.env = NULL; // Env başta boş

	init_signal();
	init_env(&data, envp); // ✅ ENV LİSTESİNİ BURADA OLUŞTURUYORSUN

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

		// Lexer
		data.tokens = lexer(line, &data);
		if (!data.tokens)
		{
			free(line);
			continue;
		}
		if (!validate_syntax(data.tokens, &data))
		{
			free_token_list(data.tokens);
			free(line);
			continue;
		}
		expand_tokens(data.env, data.tokens, data.exit_status);

		cmds = parse_tokens(data.tokens);
		if (!cmds)
		{
			free_token_list(data.tokens);
			free(line);
			continue;
		}

		executor_execute(cmds, &data);

		free_token_list(data.tokens);
		// free_cmd_list(cmds); 
		free(line);
	}
	return (0);
}
