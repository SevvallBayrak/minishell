/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sbayrak <sbayrak@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/25 07:00:04 by sbayrak           #+#    #+#             */
/*   Updated: 2025/07/26 07:06:07 by sbayrak          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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

static int readline_lexer(t_data *data)
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
static int parser_exec(t_data *data)
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

// Önerilen Düzeltilmiş Yapı
int main(int argc, char **argv, char **envp)
{
    t_data  data;
    int     i;

    data_node_null_and_init_sigenv(argc, argv, envp, &data);
    while (1)
    {
        // Bellek burada ayrılıyor
        i = readline_lexer(&data);

        if (i == 0) // Döngüden çıkış (EOF)
            break;

        // Lexer'da hata olsa bile parser'ı çalıştırmadan temizliğe git
        if (i != 1) 
        {
            parser_exec(&data);
        }

        // --- TEMİZLİK BLOGU ---
        // Bu blok artık hata durumlarında (continue yerine) bile çalışacak.
        free_token_list(data.tokens);
        if (data.cmds)
        {
            free_cmd_list(data.cmds);
            data.cmds = NULL;
        }
        free(data.raw_input);
    }

    // En son genel temizlik
    free_env(data.env);
    return (0);
}
