/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcat.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: palaca <pln.alaca21@gmail.com>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/08 17:53:04 by palaca            #+#    #+#             */
/*   Updated: 2024/10/20 20:40:09 by palaca           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

size_t	ft_strlcat(char *dst, const char *src, size_t dstsize)
{
	size_t	i;
	size_t	j;
	size_t	destlen;
	size_t	srclen;

	destlen = ft_strlen(dst);
	srclen = ft_strlen(src);
	i = 0;
	if (dstsize <= destlen)
		return (dstsize + srclen);
	j = destlen;
	while (src[i] && destlen < dstsize - 1)
	{
		dst[destlen] = src[i];
		destlen++;
		i++;
	}
	dst[destlen] = '\0';
	return (j + srclen);
}
