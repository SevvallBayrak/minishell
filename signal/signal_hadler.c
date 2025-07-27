/* ************************************************************************** */
/* */
/* :::      ::::::::   */
/* signal_hadler.c                                    :+:      :+:    :+:   */
/* +:+ +:+         +:+     */
/* By: sbayrak <sbayrak@student.42.fr>            +#+  +:+       +#+        */
/* +#+#+#+#+#+   +#+           */
/* Created: 2025/07/25 06:48:14 by sbayrak           #+#    #+#             */
/* Updated: 2025/07/27 06:39:18 by sbayrak          ###   ########.fr       */
/* */
/* ************************************************************************** */

#include "minishell.h"
#include <unistd.h>    // write için
#include <stdlib.h>    // exit için
#include <stdio.h>     // perror için
#include <signal.h>    // signal için
#include <termios.h>   // tcgetattr, tcsetattr, ECHOCTL için
#include <readline/readline.h> // rl_on_new_line, rl_replace_line, rl_redisplay, rl_line_buffer için
#include <string.h>    // ft_strlen için (veya kendi ft_strlen'in)

// rl_line_buffer global değişkeni readline/readline.h içinde tanımlıdır.
extern char *rl_line_buffer; // <-- BU SATIR ÖNEMLİ: rl_line_buffer'ı bildirir

// Global flag for SIGQUIT handling
volatile sig_atomic_t g_sigquit_flag = 0;

// Ctrl+C (SIGINT) için işleyici
void    ft_ctrl_c(int sig)
{
    (void)sig;
    write(1, "\n", 1);
    rl_on_new_line();
    rl_replace_line("", 0);
    rl_redisplay();
}

// Ctrl+\ (SIGQUIT) için özel işleyici
void    ft_ctrl_bs(int sig)
{
    (void)sig;

    // rl_get_line_buffer() yerine doğrudan rl_line_buffer değişkenini kullan
    // rl_line_buffer, readline'ın mevcut giriş tamponunu tutan global bir değişkendir.
    if (rl_line_buffer && ft_strlen(rl_line_buffer) > 0) // <-- BURASI DÜZELTİLDİ!
    {
        write(STDERR_FILENO, "Quit: \n", 8);
        g_sigquit_flag = 1;  // Flag'i set et, direk exit yapma
        rl_replace_line("", 0);
        rl_redisplay();
    }
    else
    {
        rl_on_new_line();
        rl_redisplay();
    }
}

// Sinyal işleyicilerini başlangıçta ayarlar
void    init_signal(void)
{
    struct termios  term;

    tcgetattr(STDIN_FILENO, &term);
    term.c_lflag &= ~ECHOCTL;
    tcsetattr(STDIN_FILENO, TCSANOW, &term);
    
    signal(SIGINT, ft_ctrl_c);
    signal(SIGQUIT, ft_ctrl_bs);
}

// Komutlar yürütülürken sinyalleri yoksayar
void    set_ignore_signals(void)
{
    signal(SIGINT, SIG_IGN);
    signal(SIGQUIT, SIG_IGN);
}

// Çocuk süreçler için sinyalleri varsayılanına ayarlar
void    set_default_signals(void)
{
    signal(SIGINT, SIG_DFL);
    signal(SIGQUIT, SIG_DFL);
}