/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtrim.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: palaca <pln.alaca21@gmail.com>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/17 19:16:03 by palaca            #+#    #+#             */
/*   Updated: 2024/10/21 20:57:39 by palaca           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strtrim(char const *s1, char const *set)
{
	size_t	i;
	size_t	length;
	char	*str;

	i = 0;
	while (s1[i] && \
		ft_strchr(set, s1[i]) != 0)
		i++;
	length = ft_strlen(s1) - i;
	while (s1[i] && \
		ft_strchr(set, s1[i + length - 1]) != 0)
		length--;
	str = (char *) malloc(sizeof(char) * (length + 1));
	if (!str)
		return (0);
	ft_strlcpy(str, s1 + i, length + 1);
	return (str);
}
