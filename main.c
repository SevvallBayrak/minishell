#include "minishell.h"

int main(int argc, char **argv, char **envp)
{
    char        *line;

    (void)argc;
    (void)argv;
    (void)envp;

    while (1)
    {
        line = readline("minishell> ");

        if (!line)
        {
            printf("exit\n");
            break;
        }

        if (*line)
            add_history(line);

        if (check_unclosed_quotes(line))
        {
            printf("Syntax error: unclosed quotes\n");
            free(line);
            continue;
        }
        printf("sıkıntı yook");
    }

    return (0);
}
