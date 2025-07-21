#include "minishell.h"
#include "parser.h"
#include <stdio.h>
#include <stdlib.h>
#include <readline/readline.h>
#include <readline/history.h>

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

// void	print_cmd_list(t_cmd *cmds)
// {
// 	int i;
// 	int index = 1;

// 	while (cmds)
// 	{
// 		printf("🔹 Command Block %d:\n", index++);

// 		// Argüman listesi (argv)
// 		i = 0;
// 		if (cmds->argv)
// 		{
// 			printf("  argv: ");
// 			while (cmds->argv[i])
// 				printf("[%s] ", cmds->argv[i++]);
// 			printf("\n");
// 		}
// 		else
// 			printf("  argv: (null)\n");

// 		// Redirection'lar
// 		if (cmds->infile)
// 			printf("  infile: %s\n", cmds->infile);
// 		if (cmds->outfile)
// 			printf("  outfile: %s (append=%d)\n", cmds->outfile, cmds->append);
// 		if (cmds->is_heredoc)
// 			printf("  heredoc_delim: %s\n", cmds->heredoc_delim);

// 		// Sonraki komuta geç
// 		cmds = cmds->next;
// 		printf("\n");
// 	}
// }

// void print_tokens(t_token *tokens)
// {
//     while (tokens)
//     {
//         printf("Token → Type: %d, Value: %s\n", tokens->type, tokens->value);
//         tokens = tokens->next;
//     }
// }
void data_node_null_and_init_sigenv(int argc, char **argv, char **envp, t_data *data)
{
	(void)argc;
	(void)argv;
	data->tokens = NULL;
	data->env = NULL; // Env başta boş
	data->exit_status = 0;
	data->raw_input = NULL;

	init_signal();
	init_env(data, envp);
}

int readline_lexer(t_data *data)
{
		data->raw_input = readline("minishell> ");
		if (!data->raw_input)
		{
			printf("exit\n");
			return (0);
		}
		if (*data->raw_input)
			add_history(data->raw_input);
		data->tokens = lexer(data->raw_input, data);
		if (!data->tokens)
		{
			free(data->raw_input);
			return (1);
		}
		if (!validate_syntax(data->tokens, data))
		{
			free_token_list(data->tokens);
			free(data->raw_input);
			return (1);
		}
		expand_tokens(data->env, data->tokens, data->exit_status);
		return (2);
}
int parser_exec(t_data *data)
{
	data->cmds = parse_tokens(data->tokens);
		if (!data->cmds)
		{
			free_token_list(data->tokens);
			free(data->raw_input);
			return (1);
		}
		if (has_pipe(data->cmds))
			run_pipeline(data->cmds, data);
		else
		executor_execute(data);
	return (2);

}

int	main(int argc, char **argv, char **envp)
{
	t_data	data;
	int		i;

	data_node_null_and_init_sigenv(argc, argv, envp, &data);
	while (1)
	{
		i = readline_lexer(&data);
		if (i == 0)
			break;
		else if (i == 1)
			continue;
		i = parser_exec(&data);
		if (i == 1)
			continue;
		free_token_list(data.tokens);
		if (data.cmds)
		{
    		free_cmd_list(data.cmds);
    		data.cmds = NULL;
		}
		free(data.raw_input);
	}
	free_env(data.env);
	return (0);
}
