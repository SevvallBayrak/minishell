/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstmap_bonus.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: palaca <pln.alaca21@gmail.com>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/28 04:49:01 by palaca            #+#    #+#             */
/*   Updated: 2024/10/30 23:19:18 by palaca           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

t_list	*ft_lstmap(t_list *lst, void *(*f)(void *), void (*del)(void *))
{
	t_list	*t;
	t_list	*p;

	if (!f || !lst)
		return (NULL);
	t = NULL;
	while (lst)
	{
		p = ft_lstnew((*f)(lst->content));
		if (!p)
		{
			ft_lstclear(&p, del);
			free(p);
			return (NULL);
		}
		ft_lstadd_back(&t, p);
		lst = lst->next;
	}
	return (t);
}
