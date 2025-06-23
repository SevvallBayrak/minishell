/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strjoin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: palaca <pln.alaca21@gmail.com>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/17 18:59:07 by palaca            #+#    #+#             */
/*   Updated: 2024/10/25 04:35:13 by palaca           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strjoin(char const *s1, char const *s2)
{
	size_t	i;
	size_t	j;
	size_t	p;
	char	*str;

	p = 0;
	i = ft_strlen(s1);
	j = ft_strlen(s2);
	if (!s1 || !s2)
		return (NULL);
	str = (char *) malloc((i + j) * sizeof(char) + 1);
	if (!str)
		return (NULL);
	ft_memcpy(str, s1, i);
	while (s2[p])
	{
		str[i] = s2[p];
		i++;
		p++;
	}
	str[i] = '\0';
	return (str);
}
