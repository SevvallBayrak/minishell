#include "minishell.h"
#include "utils.h"
#include "parser.h"

int	handle_word(char *input, t_token **tokens, int i)
{
	int	start;
    
    start = i;
	while (input[i] && !is_special_char(input[i]))
		i++;

	if (i > start)
	{
		char *word = ft_strndup(&input[start], i - start); // Kopyala
		add_token(tokens, word, T_WORD);               // Token ekle
		free(word);
	}
	return (i - start); // Kaç karakter ilerledik
}
int handle_quote(char *input, t_token **tokens, int i)
{
    int     start;
    char    quote;
    char    *word;

    quote = input[i]; // ' veya "
    start = ++i;       // tırnaktan sonrası

    while (input[i] && input[i] != quote)
        i++;

    if (!input[i])
    {
        write(2, "minishell: syntax error: unclosed quote\n", 40);
        return (0); // quote kapatılmamış, syntax error
    }

    word = ft_strndup(&input[start], i - start);
    if (!word)
        return (0);
    add_token(tokens, word, T_WORD);
    free(word);
    return (i - start + 2); // +1 tırnak, +1 başta artırmıştık
}


int	is_invalid_redir(const char *input, int i)
{
	if (input[i] == '<' && input[i + 1] == '<' && input[i + 2] == '<')
	{
		write(2, "minishell: syntax error\n", 54);
		return (1);
	}
	if (input[i] == '>' && input[i + 1] == '>' && input[i + 2] == '>')
	{
		write(2, "minishell: syntax error\n", 54);
		return (1);
	}
	return (0);
}
int handle_redirection(char *input, t_token **tokens, int i)
{
	// handle_redirection içinde:
	if (is_invalid_redir(input, i))
		return (0);

	if (input[i] == '>' && input[i + 1] == '>')
	{
		add_token(tokens, ">>", T_REDIR_APPEND);
		return (2); // iki karakter ilerle
	}
	else if (input[i] == '<' && input[i + 1] == '<')
	{
		add_token(tokens, "<<", T_HEREDOC);
		return (2);
	}
	else if (input[i] == '>')
	{
		add_token(tokens, ">", T_REDIR_OUT);
		return (1);
	}
	else if (input[i] == '<')
	{
		add_token(tokens, "<", T_REDIR_IN);
		return (1);
	}
	return (0); // geçerli değilse (teorik olarak gerekmez)
}

