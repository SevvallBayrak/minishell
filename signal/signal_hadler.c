#include "minishell.h"
#include <signal.h>
#include <readline/readline.h>
#include <readline/history.h>

void	sigint_handler(int sig)
{
    (void)sig;
    write(1, "\n", 1);
    rl_replace_line("", 0);       // satırı temizle
    rl_on_new_line();             // yeni satıra geç
    rl_redisplay();               // prompt'u tekrar yaz
}

void	setup_signal_handlers(void)
{
    signal(SIGINT, sigint_handler);
    signal(SIGQUIT, SIG_IGN);
}
