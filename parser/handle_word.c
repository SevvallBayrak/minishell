#include "minishell.h"
#include <stdlib.h>

int	ft_isspace(char c)
{
	return (c == 32 || (c <= 13 && c >= 9));
}
int	is_special_char(char c)
{
	return (isspace(c) || c == '|' || c == '<' || c == '>' || c == '\'' || c == '"');
}

int	handle_word(char *input, t_token **tokens, int i)
{
	int	start = i;

	while (input[i] && !is_special_char(input[i]))
		i++;

	if (i > start)
	{
		char *word = strndup(&input[start], i - start); // Kopyala
		add_token(tokens, word, T_WORD);               // Token ekle
		free(word);
	}
	return (i - start); // Kaç karakter ilerledik
}