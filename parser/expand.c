/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sbayrak <sbayrak@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/25 05:58:44 by sbayrak           #+#    #+#             */
/*   Updated: 2025/07/26 23:38:02 by sbayrak          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	append_and_replace(char **result, char *addition)
{
	char	*temp;

	temp = ft_strjoin(*result, addition);
	free(*result);
	*result = temp;
}

static char	*expand_key(t_data	*data, char *str, int i, int *j)
{
	char	*key;
	char	*value;
	char	*result;

	*j = i + 1;
	if (str[*j] == '?')
	{
		key = ft_itoa(data->exit_status);
		(*j)++;
		return (key);
	}
	while (str[*j] && (ft_isalnum(str[*j]) || str[*j] == '_'))
		(*j)++;
	key = ft_substr(str, i + 1, *j - i - 1);
	value = get_env_value(data->env, key);
	free(key);
	if (value)
		result = ft_strdup(value);
	else
		result = ft_strdup("");
	return (result);
}

char	*expand_variable(t_data *data, char *str, int i)
{
	char	*result;
	int		j;
	char	*prefix;
	char	*key;

	result = ft_strdup("");
	while (str[i])
	{	
		if (str[i] == '$' && str[i + 1]
			&& (ft_isalnum(str[i + 1]) || str[i + 1] == '_'
				|| str[i + 1] == '?'))
		{
			prefix = ft_substr(str, 0, i);
			append_and_replace(&result, prefix);
			free(prefix);
			key = expand_key(data, str, i, &j);
			append_and_replace(&result, key);
			free(key);
			str += j;
			i = 0;
		}
		else
			i++;
	}
	return (append_and_replace(&result, str), result);
}

void	expand_tokens(t_data *data, t_token *tokens)
{
	char	*expanded;

	while (tokens)
	{
		if (tokens->type == T_WORD && tokens->quote_type != 1
			&& ft_strchr(tokens->value, '$'))
		{
			expanded = expand_variable(data, tokens->value, 0);
			free(tokens->value);
			tokens->value = expanded;
		}
		tokens = tokens->next;
	}
}
