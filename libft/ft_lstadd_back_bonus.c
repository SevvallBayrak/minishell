/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstadd_back_bonus.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: palaca <pln.alaca21@gmail.com>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/26 16:22:28 by palaca            #+#    #+#             */
/*   Updated: 2024/10/30 06:50:14 by palaca           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_lstadd_back(t_list **lst, t_list *n)
{
	t_list	*s;

	if (!lst || !n)
		return ;
	if (*lst == NULL)
		*lst = n;
	else
	{
		s = ft_lstlast(*lst);
		s->next = n;
	}
}
