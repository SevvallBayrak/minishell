#include "minishell.h"

int main(int argc, char **argv, char **envp)
{
    (void)argc;
    (void)argv;
    (void)envp;

    setup_signal_handlers(); // burada çağır

    while (1)
    {
        char *line = readline("minishell> ");
        if (!line)
        {
            printf("exit\n");
            break;
        }

        if (*line)
            add_history(line);

        free(line);
    }

    return (0);
}
