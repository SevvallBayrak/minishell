#ifndef MINISHELL_H
# define MINISHELL_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include <unistd.h>
#include <readline/readline.h>
#include <readline/history.h>

// Shell state structure
typedef struct s_shell_state {
    int last_exit_status;
    char **env_vars;  // Custom environment if needed
} t_shell_state;

// Function prototypes
int check_unclosed_quotes(const char *str);
char **tokenize_input(const char *input, t_shell_state *shell);
char *expand_variable(const char *token, t_shell_state *shell);
int is_special(char c);

// Helper functions
void free_tokens(char **tokens);
void init_shell_state(t_shell_state *shell);
void cleanup_shell_state(t_shell_state *shell);

#endif