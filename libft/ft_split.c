/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pelin <pelin@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/28 11:37:52 by palaca            #+#    #+#             */
/*   Updated: 2025/06/23 16:34:45 by pelin            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static char	**ft_free(char **res)
{
	int	i;

	i = 0;
	while (res[i])
	{
		free(res[i]);
		i++;
	}
	free(res);
	return (NULL);
}

static int	ft_wrd_count(char const *s, char c)
{
	int	i;
	int	count;

	i = 0;
	count = 0;
	while (s[i])
	{
		if (s[i] != c && (s[i + 1] == c || s[i + 1] == '\0'))
			count++;
		i++;
	}
	return (count);
}

static char	**ft_res(char const *s, char c, int j, char **res)
{
	int	i;
	int	p;
	int	start;

	i = 0;
	start = 0;
	while (i < j)
	{
		while (s[start] == c)
			start++;
		p = start;
		while (s[p] != c && s[p])
			p++;
		res[i] = ft_substr(s, start, (p - start));
		if (!res[i])
			return (ft_free(res));
		i++;
		start = p;
	}
	res[i] = NULL;
	return (res);
}

char	**ft_split(char const *s, char c)
{
	char	**res;
	int		j;

	if (!s)
		return (0);
	j = ft_wrd_count(s, c);
	res = (char **)malloc((sizeof(char *)) * (j + 1));
	if (!res)
		return (0);
	res = ft_res(s, c, j, res);
	return (res);
}
