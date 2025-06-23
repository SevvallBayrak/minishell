#include "minishell.h"


// Basit echo implementasyonu
static void execute_echo(char **tokens) {
    int i = 1;
    int newline = 1;
    
    // -n flag kontrolü
    if (tokens[1] && ft_strncmp(tokens[1], "-n", ft_strlen(tokens[1])) == 0) {
        newline = 0;
        i = 2;
    }
    
    // Arguments yazdır
    while (tokens[i]) {
        if (i > (newline ? 1 : 2))
            printf(" ");
        printf("%s", tokens[i]);
        i++;
    }
    
    if (newline)
        printf("\n");
}

// Basit export implementasyonu
static void execute_export(char **tokens, t_shell_state *shell) {
    if (!tokens[1]) {
        printf("export: usage: export VAR=value\n");
        shell->last_exit_status = 1;
        return;
    }
    
    char *var = ft_strdup(tokens[1]);
    char *value = ft_strchr(var, '=');
    
    if (!value) {
        printf("export: usage: export VAR=value\n");
        free(var);
        shell->last_exit_status = 1;
        return;
    }
    
    *value = '\0';
    value++;
    
    if (setenv(var, value, 1) == 0) {
        printf("Exported: %s=%s\n", var, value);
        shell->last_exit_status = 0;
    } else {
        perror("export");
        shell->last_exit_status = 1;
    }
    
    free(var);
}

static void execute_command(char **tokens, t_shell_state *shell) {
    if (!tokens[0])
        return;
    
    if (strcmp(tokens[0], "echo") == 0) {
        execute_echo(tokens);
        shell->last_exit_status = 0;
    }
    else if (ft_strncmp(tokens[0], "export", ft_strlen(tokens[0])) == 0) {
        execute_export(tokens, shell);
    }
    else if (ft_strncmp(tokens[0], "pwd", ft_strlen(tokens[0])) == 0) {
        char cwd[1024];
        if (getcwd(cwd, sizeof(cwd))) {
            printf("%s\n", cwd);
            shell->last_exit_status = 0;
        } else {
            perror("pwd");
            shell->last_exit_status = 1;
        }
    }
    else if (ft_strncmp(tokens[0], "env", ft_strlen(tokens[0])) == 0) {
        extern char **environ;
        for (int i = 0; environ[i]; i++)
            printf("%s\n", environ[i]);
        shell->last_exit_status = 0;
    }
    else {
        shell->last_exit_status = 127;
    }
}

int main(void)
{
    char *input;
    char **tokens;
    //int i;
    t_shell_state shell;
    
    // Shell state initialize et
    init_shell_state(&shell);
    
    while (1)
    {
        input = readline("minishell> ");
        if (!input) {
            printf("\nexit\n");
            break;
        }
            
        if (*input)
            add_history(input);
            
        // Exit komutu kontrolü
        if (ft_strncmp(input, "exit", ft_strlen(input)) == 0) {
            free(input);
            break;
        }
        
        if (check_unclosed_quotes(input))
        {
            printf("minishell: syntax error: unclosed quotes\n");
            shell.last_exit_status = 2;
            free(input);
            continue;
        }
        
        // Tokenize with shell state
        tokens = tokenize_input(input, &shell);
        
        // Debug: token'ları göster
        // for (i = 0; tokens[i]; i++)
        //     printf("  [%d]: '%s'\n", i, tokens[i]);
        
        // Komutu çalıştır
        execute_command(tokens, &shell);
        
        // Cleanup
        free_tokens(tokens);
        free(input);
    }
    
    cleanup_shell_state(&shell);
    clear_history();
    return shell.last_exit_status;
}