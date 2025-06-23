#include "minishell.h"

int check_unclosed_quotes(const char *str)
{
    char quote = 0;
    while (*str)
    {
        if ((*str == '\'' || *str == '"') && quote == 0)
            quote = *str;
        else if (*str == quote)
            quote = 0;
        str++;
    }
    return (quote != 0);
}

// Özel karakter kontrolü
int is_special(char c) {
    return (c == '|' || c == '<' || c == '>');
}

// Tırnaklı token kopyalama - variable expansion ile
static char *copy_quoted_token(const char **str, t_shell_state *shell) {
    char quote = *(*str)++; // ' veya "
    const char *start = *str;
    int len = 0;
    
    while (**str && **str != quote) {
        (*str)++;
        len++;
    }
    
    char *token = ft_strndup(start, len);
    if (**str == quote)
        (*str)++; // Kapanış tırnağını atla
    
    // Çift tırnak içinde variable expansion yap
    if (quote == '"') {
        char *expanded = expand_variable(token, shell);
        free(token);
        return expanded;
    }
    
    // Tek tırnak içinde expansion yapma
    return token;
}

// Normal token kopyalama - variable expansion ile
static char *copy_normal_token(const char **str, t_shell_state *shell) {
    const char *start = *str;
    while (**str && !isspace(**str)/*!!*/ && !is_special(**str) && **str != '\'' && **str != '"')
        (*str)++;
    
    char *token = ft_strndup(start, *str - start);
    
    // Variable expansion yap
    char *expanded = expand_variable(token, shell);
    free(token);
    return expanded;
}

// Güncellenmiş tokenize fonksiyonu
char **tokenize_input(const char *input, t_shell_state *shell) {
    char *str = ft_strdup(input);
    const char *p = str;
    int capacity = 10;
    char **tokens = malloc(capacity * sizeof(char *));
    int count = 0;

    while (*p) {
        // Boşlukları atla
        while (isspace(*p))//!!!!
            p++;
        
        if (!*p)
            break;

        char *token = NULL;
        
        if (*p == '\'' || *p == '"') {
            // Tırnaklı token
            token = copy_quoted_token(&p, shell);
        }
        else if (is_special(*p)) {
            // Özel karakterler (pipe, redirection)
            if (*p == '>' && *(p + 1) == '>') {
                token = ft_strdup(">>");
                p += 2;
            }
            else if (*p == '<' && *(p + 1) == '<') {
                token = ft_strdup("<<");
                p += 2;
            }
            else {
                token = malloc(2);
                token[0] = *p;
                token[1] = '\0';
                p++;
            }
        }
        else {
            // Normal token
            token = copy_normal_token(&p, shell);
        }

        // Token array'ini genişlet
        if (count >= capacity) {
            capacity *= 2;
            tokens = realloc(tokens, capacity * sizeof(char *));//!!!
        }
        
        tokens[count++] = token;
    }
    
    tokens[count] = NULL;
    free(str);
    return tokens;
}

// Utility functions
void free_tokens(char **tokens) {
    if (!tokens)
        return;
    
    for (int i = 0; tokens[i]; i++)
        free(tokens[i]);
    free(tokens);
}

void init_shell_state(t_shell_state *shell) {
    shell->last_exit_status = 0;
    shell->env_vars = NULL;
}

void cleanup_shell_state(t_shell_state *shell) {
    if (shell->env_vars) {
        for (int i = 0; shell->env_vars[i]; i++)
            free(shell->env_vars[i]);
        free(shell->env_vars);
    }
}