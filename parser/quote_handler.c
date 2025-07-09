#include "parser.h"

int check_unclosed_quotes(const char *str)
{
    char flag = 0;
    while (*str)
    {
        if ((*str == '\'' || *str == '"') && flag == 0)
            flag = *str;
        else if (*str == flag)
            flag = 0;
        str++;
    }
    return (flag != 0);
}
int	validate_syntax(t_token *tok)
{
	if (!tok)
		return (0);
	// PIPE ile başlama
	if (tok->type == T_PIPE)
	{
		write(2, "minishell: syntax error near unexpected token `|'\n", 50);
		return (0);
	}
	while (tok)
	{
		// PIPE sonrası bir şey yoksa
		if (tok->type == T_PIPE && (!tok->next || tok->next->type == T_PIPE))
		{
			write(2, "minishell: syntax error near unexpected token `|'\n", 50);
			return (0);
		}
		// REDIRECTION sonrası kelime yoksa
		if (tok->type >= T_REDIR_IN && tok->type <= T_HEREDOC)
		{
			if (!tok->next || tok->next->type != T_WORD)
			{
				write(2, "minishell: syntax error near unexpected token `newline'\n", 57);
				return (0);
			}
		}
		tok = tok->next;
	}
	return (1);
}
