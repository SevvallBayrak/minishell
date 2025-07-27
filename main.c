/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sbayrak <sbayrak@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/25 07:00:04 by sbayrak           #+#    #+#             */
/*   Updated: 2025/07/27 22:33:01 by sbayrak          ###   ########.fr       */
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

void	init_data_node(int argc, char **argv, t_data *data)
{
	(void)argc;
	(void)argv;
	data->tokens = NULL;
	data->env = NULL;
	data->exit_status = 0;
	data->raw_input = NULL;
}

void	data_node_null_and_init_sigenv(int argc, char **argv,
						char **envp, t_data *data)
{
	init_data_node(argc, argv, data);
	init_signal();
	init_env(data, envp);
}

static int	readline_lexer_part2(t_data *data)
{
	if (!validate_syntax(data->tokens, data))
	{
		free_token_list(data->tokens);
		free(data->raw_input);
		return (1);
	}
	expand_tokens(data, data->tokens);
	return (2);
}

static int	readline_lexer(t_data *data)
{
	data->raw_input = readline("minishell> ");
	if (g_sigquit_flag)
	{
		g_sigquit_flag = 0;
		if (data->raw_input)
			free(data->raw_input);
		return (-1);
	}
	if (!data->raw_input)
		return (0);
	if (!(*data->raw_input))
	{
		free(data->raw_input);
		return (3);
	}
	add_history(data->raw_input);
	data->tokens = lexer(data);
	if (!data->tokens)
	{
		free(data->raw_input);
		return (1);
	}
	return (readline_lexer_part2(data));
}

static int	parser_exec(t_data *data)
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

void	exit_and_free(t_data *data)
{
	rl_clear_history();
	if (data->env)
		free_env(data->env);
	if (data->exported_vars)
		free_env(data->exported_vars);
	exit(131);
}

void	main_loop(t_data *data)
{
	int	i;

	while (1)
	{
		i = readline_lexer(data);
		if (i == 0)
			break ;
		if (i == -1)
			exit_and_free(data);
		if (i == 1 || i == 3)
			continue ;
		if (i != 1)
			parser_exec(data);
		free_token_list(data->tokens);
		if (data->cmds)
		{
			free_cmd_list(data->cmds);
			data->cmds = NULL;
		}
		if (ft_strlen(data->raw_input) != 0)
			free(data->raw_input);
	}
}

int	main(int argc, char **argv, char **envp)
{
	t_data	data;

	data_node_null_and_init_sigenv(argc, argv, envp, &data);
	main_loop(&data);
	free_env(data.env);
	if (data.exported_vars)
		free_env(data.exported_vars);
	return (0);
}
