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

void	free_cmd_list(t_cmd *cmd)
{
	t_cmd	*tmp;

	while (cmd)
	{
		tmp = cmd->next;
		free_argv(cmd->argv);
		if (cmd->infile)
			free(cmd->infile);
		if (cmd->outfile)
			free(cmd->outfile);
		if (cmd->heredoc_delim)
			free(cmd->heredoc_delim);
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

void	data_node_null_and_init_sigenv(int argc, char **argv,
							char **envp, t_data *data)
{
	(void)argc;
	(void)argv;
	data->tokens = NULL;
	data->env = NULL;
	data->exit_status = 0;
	data->raw_input = NULL;
	init_signal();
	init_env(data, envp);
}

static int	readline_lexer(t_data *data)
{
	data->raw_input = readline("minishell> ");
	if (!data->raw_input)
	{
		printf("exit\n");
		return (0);
	}
	add_history(data->raw_input);
	data->tokens = lexer(data);
	//print_tokens(data->tokens);
	if (!data->tokens)
	{
		if (data->raw_input)
			free(data->raw_input);
		return (1);
	}
	if (!validate_syntax(data->tokens, data))
	{
		free_token_list(data->tokens);
		free(data->raw_input);
		return (1);
	}
	expand_tokens(data, data->tokens);
	return (2);
}

static int	parser_exec(t_data *data)
{
	data->cmds = parse_tokens(data->tokens);
	//print_cmd_list(data->cmds);
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
			break ;
		if (i != 1)
			parser_exec(&data);
		free_token_list(data.tokens);
		if (data.cmds)
		{
			free_cmd_list(data.cmds);
			data.cmds = NULL;
		}
		if (ft_strlen(data.raw_input) != 0)
			free(data.raw_input);
	}
	free_env(data.env);
	return (0);
}

/*

minishell> ls | pwd
/home/palaca/Desktop/minik
==340231== 
==340231== FILE DESCRIPTORS: 3 open (3 std) at exit.
==340231== 
==340231== HEAP SUMMARY:
==340231==     in use at exit: 212,061 bytes in 421 blocks
==340231==   total heap usage: 914 allocs, 493 frees, 239,209 bytes allocated
==340231== 
==340231== LEAK SUMMARY:
==340231==    definitely lost: 0 bytes in 0 blocks
==340231==    indirectly lost: 0 bytes in 0 blocks
==340231==      possibly lost: 0 bytes in 0 blocks
==340231==    still reachable: 212,061 bytes in 421 blocks
==340231==         suppressed: 0 bytes in 0 blocks
==340231== Rerun with --leak-check=full to see details of leaked memory
==340231== 
==340231== For lists of detected and suppressed errors, rerun with: -s
==340231== ERROR SUMMARY: 0 errors from 0 contexts (suppressed: 0 from 0)
==340228== Syscall param wait4(pid) contains uninitialised byte(s)
==340228==    at 0x49B03EA: wait4 (wait4.c:30)
==340228==    by 0x405D4E: wait_all_children (pipe_utils.c:76)
==340228==    by 0x403433: run_pipeline (pipeline.c:80)
==340228==    by 0x403BBE: parser_exec (main.c:143)
==340228==    by 0x4039CB: main (main.c:161)
==340228== 
==340230== 
==340230== FILE DESCRIPTORS: 3 open (3 std) at exit.
==340230== 
==340230== HEAP SUMMARY:
==340230==     in use at exit: 208,259 bytes in 228 blocks
==340230==   total heap usage: 975 allocs, 747 frees, 241,834 bytes allocated
==340230== 
==340230== LEAK SUMMARY:
==340230==    definitely lost: 0 bytes in 0 blocks
==340230==    indirectly lost: 0 bytes in 0 blocks
==340230==      possibly lost: 0 bytes in 0 blocks
==340230==    still reachable: 208,259 bytes in 228 blocks
==340230==         suppressed: 0 bytes in 0 blocks
==340230== Rerun with --leak-check=full to see details of leaked memory
==340230== 
==340230== For lists of detected and suppressed errors, rerun with: -s
==340230== ERROR SUMMARY: 0 errors from 0 contexts (suppressed: 0 from 0)
minishell> 
*/

/*
ilk boş enterda bum
export a yapıp export yapınca yazmalı ama envye yazmıcak
*/