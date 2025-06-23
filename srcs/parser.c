#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <ctype.h>
#include <stdbool.h>

int	check_unclosed_quotes(const char *str)
{
	char	quote = 0;

	while (*str)
	{
		if ((*str == '\'' || *str == '"') && quote == 0)
			quote = *str;
		else if (*str == quote)
			quote = 0;
		str++;
	}
	return (quote != 0);
}

int is_special(char c)
{
	return (c == '|' || c == '<' || c == '>');
}
static char *copy_quoted_token(const char **str)
{
	char quote = *(*str)++; // ' veya "
	const char *start = *str;
	int len = 0;

	while (**str && **str != quote)
	{
		(*str)++;
		len++;
	}

	char *token = strndup(start, len);
	if (**str == quote)
		(*str)++; // kapanış tırnağını atla
	return token;
}

static int	count_tokens(const char *str)
{
	int count = 0;

	while (*str)
	{
		while (isspace(*str))
			str++;
		if (!*str)
			break;
		if (is_special(*str))
		{
			count++;
			str++;
		}
		else
		{
			count++;
			while (*str && !isspace(*str) && !is_special(*str))
				str++;
		}
	}
	return (count);
}

static char	*copy_token(const char **str)
{
	const char *start;
	int len = 0;

	while (isspace(**str))
		(*str)++;

	if (**str == '\'' || **str == '"')
		return copy_quoted_token(str);

	if (is_special(**str))
	{
		char *tok = malloc(2);
		tok[0] = *(*str)++;
		tok[1] = '\0';
		return tok;
	}
	start = *str;
	while (**str && !isspace(**str) && !is_special(**str) && **str != '\'' && **str != '"')
	{
		(*str)++;
		len++;
	}
	return strndup(start, len);
}


char **tokenize_input(const char *input)
{
	int count = count_tokens(input);
	char **tokens = malloc(sizeof(char *) * (count + 1));
	int i = 0;

	while (*input)
	{
		while (isspace(*input))
			input++;
		if (!*input)
			break;
		tokens[i++] = copy_token(&input);
	}
	tokens[i] = NULL;
	return tokens;
}
