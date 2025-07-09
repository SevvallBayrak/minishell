#include "minishell.h"

char *expand_exit_status(char *str, int pos, int exit_status)
{
	char *status = ft_itoa(exit_status);
	// ...
}



char *expand_variable(t_env *env, char *str)
{
	int		i = 0, j;
	char	*key, *value, *prefix, *suffix, *result;

	while (str[i])
	{
		if (str[i] == '$' && str[i + 1] && (ft_isalnum(str[i + 1]) || str[i + 1] == '_' || str[i + 1] == '?'))
		{
			j = i + 1;
			if (str[j] == '?')
				return expand_exit_status(str, i); // $?

			while (str[j] && (ft_isalnum(str[j]) || str[j] == '_'))
				j++;

			key = ft_substr(str, i + 1, j - (i + 1));
			value = get_env_value(env, key);
			free(key);

			prefix = ft_substr(str, 0, i);
			suffix = ft_strdup(&str[j]);
			result = ft_strjoin(prefix, value ? value : "");
			free(prefix);
			key = ft_strjoin(result, suffix);
			free(result);
			free(suffix);
			return key;
		}
		i++;
	}
	return ft_strdup(str);
}


void expand_tokens(t_env *env, t_token *tokens)
{
	while (tokens)
	{
		if (tokens->type == T_WORD && tokens->quote_type != 1)
		{
			if (ft_strchr(tokens->value, '$'))
			{
				char *expanded = expand_variable(env, tokens->value);
				free(tokens->value);
				tokens->value = expanded;
			}
		}
		tokens = tokens->next;
	}
}
