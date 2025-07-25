/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sbayrak <sbayrak@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/25 06:55:36 by sbayrak           #+#    #+#             */
/*   Updated: 2025/07/25 08:16:34 by sbayrak          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "../libft/libft.h"
# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <string.h>
# include <fcntl.h>
# include <signal.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <sys/wait.h>
# include <termios.h>

extern int	g_exit_status;

typedef enum e_path_result
{
	PATH_OK = 0,
	PATH_NOT_FOUND = 127,
	PATH_NO_PERMISSION = 126
}	t_path_result;

typedef enum e_token_type
{
	T_WORD,
	T_PIPE,
	T_REDIR_IN,
	T_REDIR_OUT,
	T_REDIR_APPEND,
	T_HEREDOC
}	t_token_type;

typedef struct s_token
{
	char			*value;
	t_token_type	type;
	int				quote_type;
	struct s_token	*next;
}	t_token;

typedef struct s_cmd
{
	char			**argv;
	char			*infile;
	char			*outfile;
	char			*heredoc_delim;
	int				append;
	int				is_heredoc;
	struct s_cmd	*next;
}	t_cmd;

typedef struct s_env
{
	char			*key;
	char			*value;
	struct s_env	*next;
}	t_env;

typedef struct s_data
{
	char	*raw_input;
	t_token	*tokens;
	t_cmd	*cmds;
	t_env	*env;
	int		exit_status;
}	t_data;

/* Signal */
void	init_signal(void);
void	setup_signal_handlers(void);
void	set_ignore_signals(void);
void	set_default_signals(void);

/* Builtin */
int		is_builtin(const char *cmd);
int		exec_builtin(t_cmd *cmd, t_data *data);
int		is_valid_identifier(const char *str);
int		is_valid_key(const char *key);
int		process_export_arg(char *arg, t_data *data);
int		ft_cd(char **argv, t_data *data);
int		ft_echo(char **argv);
int		ft_pwd(t_data *data);
int		ft_exit(char **argv, t_data *data);
int		ft_env(t_data *data);
int		ft_export(char **argv, t_data *data);
int		ft_unset(char **argv, t_data *data);

/* Env */
t_env	*find_env(t_env *env, const char *key);
void	add_or_update_env(t_data *data, const char *key, const char *value);
void	remove_env_var(t_env **env, const char *key);
char	*get_env_value(t_env *env, const char *key);
void	update_env_var(t_data *data, const char *key, const char *value);
void	env_add_back(t_env **env, t_env *new_node);
char	**env_to_envp(t_env *env);
void	init_env(t_data *data, char **envp);
void	load_env_from_envp(t_data *data, char **envp);
void	ensure_pwd_exists(t_data *data);

/* Lexer */
t_token	*lexer(char *input, t_data *data);
t_token	*create_token(char *value, int type, int quote);
void	add_token(t_token **head, char *value,
			t_token_type type, int q_type);
void	free_token_list(t_token *list);
int		handle_quote(char *input, t_token **tokens,
			int i, t_data *data);
int		handle_redirection(char *input, t_token **tokens,
			int i, t_data *data);
int		handle_word(char *input, t_token **tokens, int i);
int		print_no_red_next_word_error(t_token *next);
int		is_operator(char c);
int		is_special_char(char c);
int		ft_isspace(char c);
int		check_unclosed_quotes(const char *str);
int		dispatch_lexer(char *input, int i,
			t_token **tokens, t_data *data);
int		odd_or_double_quote(char quote);
int		print_unclosed_quote(t_data *data);

/* Parser */
t_cmd	*parse_tokens(t_token *tokens);
t_cmd	*init_new_cmd(void);
int		handle_pipe_token(t_cmd **current);
int		handle_redirection_token(t_cmd *cmd, t_token **tok);
int		handle_token(t_token **tok, t_cmd **curr);
int		handle_word_token(t_token *tok, t_cmd *current);
int		init_if_needed(t_cmd **cmds, t_cmd **current);
void	assign_redirection(t_cmd *cmd, t_token *tok);
void	open_and_close_outfile(t_cmd *cmd);
char	**append_str_to_array(char **arr, char *new_str);

/* Executor */
int		executor_execute(t_data *data);
int		execute_command(t_cmd *cmd, char **argv, t_data *data);
char	*get_command_path(char *cmd, t_data *data, int *result);
char	*handle_absolute_cmd(char *cmd, int *result);
char	*search_path_dirs(char *path_env, char *cmd, int *result);
int		update_result(char *path, int *result);
void	handle_exec_error(int result, char **envp);

/* Pipeline */
int		has_pipe(t_cmd *cmds);
int		run_pipeline(t_cmd *cmds, t_data *data);
void	pipe_child_exec(t_cmd *cmd, int in_fd, int out_fd, t_data *data);
void	start_pipeline_child(t_cmd *cmd, int *pipefd,
			int in_fd, t_data *data);
void	close_pipe_ends(int *pipefd);
int		count_commands(t_cmd *cmds);
int		wait_all_children(pid_t *pids, int num_cmds);
int		cleanup_on_error(pid_t *pids, int i, int in_fd);

/* Heredoc / Redirection */
int		handle_heredoc(t_cmd *cmd, t_data *data);
int		handle_heredoc_and_redirects(t_cmd *cmd, t_data *data);
int		is_expandable_delimiter(const char *delim);
int		prepare_stdin_for_heredoc(t_cmd *cmd, t_data *data);
int		redirect_in(t_cmd *cmd, t_data *data);
int		redirect_out(t_cmd *cmd, t_data *data);
int		validate_syntax(t_token *tok, t_data *data);

/* Expansion */
char	*expand_variable(t_env *env, char *str, int exit_status);
void	expand_tokens(t_env *env, t_token *tokens, int exit_status);

/* Utils */
char	*ft_strjoin_path(const char *dir, const char *cmd);
char	*ft_strndup(const char *s1, int a);
char	*append_char_to_str(char *s, char c);
void	restore_std_fds(int in, int out);
void	exit_cleanup(t_data *data);
void	free_argv(char **argv);
void	free_cmd_list(t_cmd *cmd);
void	free_env(t_env *env);
void	free_str_array(char **arr);

#endif
