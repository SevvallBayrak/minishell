/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strrchr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: palaca <pln.alaca21@gmail.com>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/15 17:52:47 by palaca            #+#    #+#             */
/*   Updated: 2024/10/23 21:46:46 by palaca           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

char	*ft_strrchr(const char *s, int c)
{
	int				i;
	unsigned char	a;

	i = 0;
	a = (unsigned char) c;
	while (*(s + i))
	{
		i++;
	}
	while (i >= 0)
	{
		if (s[i] != a)
		{
			i--;
		}
		else
		{
			return ((char *)s + i);
		}
	}
	return (0);
}
