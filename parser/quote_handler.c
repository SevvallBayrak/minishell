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
int	validate_syntax(t_token *tok, t_data *data)
{
	if (!tok)
		return (1); // Boşsa sorun yok
	if (tok->type == T_PIPE)
	{
		write(2, "minishell: syntax error near unexpected token `|'\n", 50);
		return (0);
	}
	while (tok)
	{
		// PIPE sonrası bir şey yoksa veya bir tane daha PIPE varsa
		if (tok->type == T_PIPE && (!tok->next || tok->next->type == T_PIPE))
		{
			write(2, "minishell: syntax error near unexpected token `|'\n", 50);
			data->exit_status = 258;
			return (0);
		}
		// Redirect'ten sonra kelime yoksa
		if (tok->type >= T_REDIR_IN && tok->type <= T_HEREDOC)
		{
			if (!tok->next || tok->next->type != T_WORD)
			{
				write(2, "minishell: syntax error near unexpected token `newline'\n", 57);
				data->exit_status = 258;
				return (0);
			}
		}
		tok = tok->next;
	}
	return (1);
}
int odd_or_double_quote(char quote)
{
    if(quote == '\'')
        return(1);
    else if(quote == '"')
        return(2);
    return(0);
}

