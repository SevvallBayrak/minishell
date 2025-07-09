#include "minishell.h"

// char *expand_exit_status(char *str, int pos, int exit_status)
// {
// 	char *status = ft_itoa(exit_status);
// 	// ...
// }

static char *expand_key(t_env *env, char *str, int i, int *j, int exit_status)
{
	char *key;
	char *value;

	*j = i + 1;
	if (str[*j] == '?')
	{
		key = ft_itoa(exit_status);
		(*j)++;
	}
	else
	{
		while (str[*j] && (ft_isalnum(str[*j]) || str[*j] == '_'))
			(*j)++;
		key = ft_substr(str, i + 1, *j - i - 1);
		value = get_env_value(env, key);
		free(key);
		key = ft_strdup(value ? value : "");
	}
	return (key);
}

static void	append_and_replace(char **result, char *addition)
{
	char *temp = ft_strjoin(*result, addition);
	free(*result);
	*result = temp;
}

char *expand_variable(t_env *env, char *str, int exit_status)
{
	int		i = 0;
	int		j;
	char	*result = ft_strdup("");
	char	*prefix;
	char	*key;

	while (str[i])
	{
		if (str[i] == '$' && str[i + 1] &&
			(ft_isalnum(str[i + 1]) || str[i + 1] == '_' || str[i + 1] == '?'))
		{
			prefix = ft_substr(str, 0, i); // $'tan önceki kısmı al
			append_and_replace(&result, prefix);
			free(prefix);

			key = expand_key(env, str, i, &j, exit_status); // $VAR ya da $?
			append_and_replace(&result, key);
			free(key);

			str += j; // işlenen kısmı atla
			i = 0; // yeni str'de sıfırdan başla
		}
		else
			i++;
	}
	append_and_replace(&result, str); // kalan kısmı ekle
	return (result);
}


void expand_tokens(t_env *env, t_token *tokens, int exit_status)
{
	char	*expanded;

	while (tokens)
	{
		if (tokens->type == T_WORD && tokens->quote_type != 1)
		{
			if (ft_strchr(tokens->value, '$'))
			{
				expanded = expand_variable(env, tokens->value, exit_status);
				free(tokens->value);
				tokens->value = expanded;
			}
		}
		tokens = tokens->next;
	}
}