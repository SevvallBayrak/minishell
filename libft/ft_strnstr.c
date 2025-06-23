/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strnstr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: palaca <pln.alaca21@gmail.com>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/16 19:34:04 by palaca            #+#    #+#             */
/*   Updated: 2024/10/28 05:20:13 by palaca           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strnstr(const char *haystack, const char *needle, size_t len)
{
	size_t	i;
	size_t	j;

	i = 0;
	j = 0;
	if (needle[i] == '\0')
	{
		return ((char *) haystack);
	}
	while (((j + i) < len) && haystack[j])
	{
		while (haystack[j + i] != needle[i])
		{
			j++;
			i = 0;
		}
		i++;
		if (((j + i) <= (len)) && needle[i] == '\0')
		{
			return ((char *) haystack + j);
		}
	}
	return (NULL);
}
