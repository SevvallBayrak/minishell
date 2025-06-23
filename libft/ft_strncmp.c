/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strncmp.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: palaca <pln.alaca21@gmail.com>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/15 18:09:04 by palaca            #+#    #+#             */
/*   Updated: 2024/10/25 16:36:17 by palaca           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_strncmp(const char *s1, const char *s2, size_t n)
{
	unsigned char	*c1;
	unsigned char	*c2;
	size_t			i;

	c1 = (unsigned char *) s1;
	c2 = (unsigned char *) s2;
	i = 0;
	while (i < n && !(c2[i] == '\0' && c1[i] == '\0'))
	{
		if (c1[i] == c2[i])
		{
			i++;
		}
		else if (c1[i] > c2[i])
			return (1);
		else
			return (-1);
	}
	return (0);
}
