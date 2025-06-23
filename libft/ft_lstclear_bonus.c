/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstclear_bonus.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: palaca <pln.alaca21@gmail.com>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/27 23:06:14 by palaca            #+#    #+#             */
/*   Updated: 2024/10/28 04:41:30 by palaca           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_lstclear(t_list **lst, void (*del)(void*))
{
	t_list	*t;
	t_list	*p;

	if (!lst)
		return ;
	t = *lst;
	while (t)
	{
		p = t->next;
		ft_lstdelone(t, del);
		t = p;
	}
	*lst = NULL;
}
