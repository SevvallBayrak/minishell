#include "minishell.h"

char	*ft_strndup(const char *s1, int a)
{
	int	i;
	char	*str;

	str = (char *)malloc((a + 1) * sizeof(char));
	if (!str)
		return (NULL);
	i = 0;
	while (i < a && s1[i])
	{
		str[i] = s1[i];
		i++;
	}
	str[i] = '\0';
	return (str);
}
int	ft_isspace(char c)
{
	return (c == 32 || (c <= 13 && c >= 9));
}
int	is_special_char(char c)
{
	return (isspace(c) || c == '|' || c == '<' || c == '>' || c == '\'' || c == '"');
}
int is_operator(char c)
{
    return (c == '>' || c == '<' || c == '|');
}