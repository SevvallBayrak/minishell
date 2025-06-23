/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: palaca <pln.alaca21@gmail.com>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/15 15:33:06 by palaca            #+#    #+#             */
/*   Updated: 2024/10/22 22:12:40 by palaca           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

char	*ft_strchr(const char *s, int c)
{
	unsigned char	expected;
	int				i;

	i = 0;
	expected = (unsigned char) c;
	while (s[i])
	{
		if (s[i] != expected)
		{
			i++;
		}
		else
		{
			return ((char *)s + i);
		}
	}
	if (s[i] == expected)
		return ((char *)s + i);
	return (0);
}
