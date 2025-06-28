// #include "../includes/minishell.h"

// //AŞKIM BALIM BİTANEEEEMMM<3

// t_env	*ft_lstnew_env(char *key, char *value)
// {
// 	t_env	*a;

// 	a = malloc(sizeof(t_env));
// 	if (!a)
// 		return (NULL);
//     a->key = key;
// 	a->value = value;
// 	a->next = NULL;
// 	return (a);
// }
// t_env	*ft_lstlast_env(t_env *lst)
// {
// 	t_env	*s;

// 	if (!lst)
// 		return (NULL);
// 	s = lst;
// 	while (s->next)
// 		s = s->next;
// 	return (s);
// }
// void	ft_lstadd_back_env(t_env **lst, t_env *n)
// {
// 	t_env	*s;

// 	if (!lst || !n)
// 		return ;
// 	if (*lst == NULL)
// 		*lst = n;
// 	else
// 	{
// 		s = ft_lstlast_env(*lst);
// 		s->next = n;
// 	}
// }

// void	ft_lstclear_env(t_list **lst, void (*del)(void*))
// {
// 	t_list	*t;
// 	t_list	*p;

// 	if (!lst)
// 		return ;
// 	t = *lst;
// 	while (t)
// 	{
// 		p = t->next;
// 		ft_lstdelone(t, del);
// 		t = p;
// 	}
// 	*lst = NULL;
// }*/