/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exported_vars_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sbayrak <sbayrak@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/28 15:54:22 by sbayrak           #+#    #+#             */
/*   Updated: 2025/07/28 16:20:39 by sbayrak          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	add_exported_var(t_data *data, const char *key)
{
	t_env	*new_node;

	if (find_exported_var(data->exported_vars, key))
		return ;
	new_node = malloc(sizeof(t_env));
	if (!new_node)
		return ;
	new_node->key = ft_strdup(key);
	new_node->value = NULL;
	new_node->next = NULL;
	env_add_back(&data->exported_vars, new_node);
}

t_env	*find_exported_var(t_env *exported_vars, const char *key)
{
	while (exported_vars)
	{
		if (ft_strncmp(exported_vars->key, key, (ft_strlen(key) + 1)) == 0)
			return (exported_vars);
		exported_vars = exported_vars->next;
	}
	return (NULL);
}
