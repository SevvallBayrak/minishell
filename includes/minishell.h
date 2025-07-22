#ifndef MINISHELL_H
# define MINISHELL_H

#include "../libft/libft.h"
# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <string.h>
# include <fcntl.h>
# include <signal.h>
# include <readline/readline.h>
# include <readline/history.h>

/* ───────────────────────────────────────────── */
/*              Global Exit Durumu              */
/* ───────────────────────────────────────────── */
extern int g_exit_status; // $? değişkeni için global durum

/* ───────────────────────────────────────────── */
/*                 Token Yapıları               */
/* ───────────────────────────────────────────── */
typedef enum e_token_type {
    T_WORD,
    T_PIPE,
    T_REDIR_IN,
    T_REDIR_OUT,
    T_REDIR_APPEND,
    T_HEREDOC
} t_token_type;

typedef struct s_token {
    char            *value;
    t_token_type    type;
    int             quote_type; //tek tırnak mı çift tırnak mı için ama şuan kullanmadık lexerda
    struct s_token  *next;
} t_token;


/* ───────────────────────────────────────────── */
/*                Komut Yapısı                  */
/* ───────────────────────────────────────────── */
typedef struct s_cmd {
    char            **argv; // Argüman listesi: ["ls", "-l", NULL]
    char            *infile; // input redirection dosyası (örnek: < input.txt)
    char            *outfile; // output redirection dosyası (örnek: > out.txt)
    char            *heredoc_delim; // << EOF → buraya delimiter yazılır
    char            *heredoc_file; // << BU SATIRI EKLE
    int             append; //	>> file.txt varsa 1, > ise 0
    int             is_heredoc; //	<< varsa 1, yoksa 0
    struct s_cmd    *next; // 	Pipe varsa 
} t_cmd;

/* ───────────────────────────────────────────── */
/*             Environment Değişkeni            */
/* ───────────────────────────────────────────── */
typedef struct s_env {
    char            *key; // = buraya dahil olucak
    char            *value;
    struct s_env    *next;
} t_env;

/* ───────────────────────────────────────────── */
/*         Tüm Yapıyı Taşıyan Ana Struct         */
/* ───────────────────────────────────────────── */
typedef struct s_data {
    char        *raw_input;    // Kullanıcının yazdığı ham komut
    t_token     *tokens;       // Token listesi (lexer çıktısı)
    t_cmd       *cmds;         // Komutlar listesi (parser çıktısı)
    t_env       *env;          // Environment değişken listesi
    int         exit_status;   // ← Global exit kodu yerine kullanılacak
} t_data;
void	setup_signal_handlers(void);
void	init_signal(void);
void	exit_cleanup(t_data *data);

int		is_builtin(const char *cmd);
int		exec_builtin(t_cmd *cmd, t_data *data);
int		ft_echo(char **args);
int		ft_cd(char **args, t_data *data);
int		ft_pwd(t_data *data);
int		ft_export(char **argv, t_data *data);
int		ft_unset(char **argv, t_data *data);
int		ft_env(t_data *data);
int		ft_exit(char **argv, t_data *data);
int		is_valid_identifier(const char *str);

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

int		executor_execute(t_data *data);
int		execute_command(t_cmd *cmd, char **argv, t_data *data);
char	*ft_strjoin_path(const char *dir, const char *cmd);

int		redirect_out(t_cmd *cmd, t_data *data);
int		redirect_in(t_cmd *cmd, t_data *data);
void	free_argv(char **argv);
void	free_cmd_list(t_cmd *cmd);
void	free_env(t_env *env);
void	free_token_list(t_token *list);

int		handle_heredoc(t_cmd *cmd, t_data *data);
int		is_expandable_delimiter(const char *delim);
char	*expand_variable(t_env *env, char *str, int exit_status);

/* Pipeline & pipe utils */
int		has_pipe(t_cmd *cmds);
int		run_pipeline(t_cmd *cmds, t_data *data);
void	pipe_child_exec(t_cmd *cmd, int in_fd, int out_fd, t_data *data);

/* --- Eklenen parçalanmış fonksiyonlar --- */
// void	assign_redirection(t_cmd *cmd, t_token *tok);
// int		init_if_needed(t_cmd **cmds, t_cmd **current);
// int		handle_word_token(t_token *tok, t_cmd *current);



#endif
