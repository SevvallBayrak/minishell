#ifndef MINISHELL_H
# define MINISHELL_H

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
   struct s_token  *next;
} t_token;

/* ───────────────────────────────────────────── */
/*                Komut Yapısı                  */
/* ───────────────────────────────────────────── */
typedef struct s_cmd {
    char            **argv;
    char            *infile;
    char            *outfile;
    char            *heredoc_delim;
    int             append;
    int             is_heredoc;
    struct s_cmd    *next;
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
} t_data;

#endif
