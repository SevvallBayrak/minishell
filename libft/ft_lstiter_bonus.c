/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstiter_bonus.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: palaca <pln.alaca21@gmail.com>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/28 04:38:10 by palaca            #+#    #+#             */
/*   Updated: 2024/10/28 04:52:53 by palaca           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_lstiter(t_list *lst, void (*f)(void *))
{
	t_list	*t;
	t_list	*p;

	if (!lst)
		return ;
	t = lst;
	while (t)
	{
		p = t->next;
		f(t->content);
		t = p;
	}
	lst = t;
}
