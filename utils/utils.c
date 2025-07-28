/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sbayrak <sbayrak@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/25 06:52:32 by sbayrak           #+#    #+#             */
/*   Updated: 2025/07/28 21:59:36 by sbayrak          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*ft_strndup(const char *s1, int a)
{
	int		i;
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
	return (ft_isspace(c) || c == '|' || c == '<'
		|| c == '>' || c == '\'' || c == '"');
}

int	is_operator(char c)
{
	return (c == '>' || c == '<' || c == '|');
}

char	*ft_strjoin_path(const char *dir, const char *cmd)
{
	char	*res;
	size_t	len;

	if (!dir || !cmd)
		return (NULL);
	len = ft_strlen(dir) + ft_strlen(cmd) + 2;
	res = malloc(len);
	if (!res)
		return (NULL);
	ft_strlcpy(res, dir, len);
	ft_strlcat(res, "/", len);
	ft_strlcat(res, cmd, len);
	return (res);
}
