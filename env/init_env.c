/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_env.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sbayrak <sbayrak@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/25 06:58:19 by sbayrak           #+#    #+#             */
/*   Updated: 2025/07/27 06:48:28 by sbayrak          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	env_add_back(t_env **env, t_env *new_node)
{
	t_env	*tmp;

	if (!env || !new_node)
		return ;
	if (!*env)
	{
		*env = new_node;
		return ;
	}
	tmp = *env;
	while (tmp->next)
		tmp = tmp->next;
	tmp->next = new_node;
}

void	freetmp_and_join(t_env *node, char **tmp, int j)
{
	char	*temp_value;

	temp_value = ft_strjoin(node->value, "=");
	free(node->value);
	node->value = ft_strjoin(temp_value, tmp[j]);
	free(temp_value);
}

void	node_value(char **tmp, t_env *node)
{
	int	j;

	node->value = ft_strdup(tmp[1]);
	j = 2;
	while (tmp[j])
	{
		freetmp_and_join(node, tmp, j);
		j++;
	}
}

void	init_env(t_data *data, char **envp)
{
	int		i;
	char	**tmp;
	t_env	*node;

	i = -1;
	while (envp[++i])
	{
		tmp = ft_split(envp[i], '=');
		if (!tmp)
			continue ;
		node = malloc(sizeof(t_env));
		if (!node)
		{
			free_argv(tmp);
			continue ;
		}
		node->key = ft_strdup(tmp[0]);
		if (tmp[1])
			node_value(tmp, node);
		else
			node->value = NULL;
		node->next = NULL;
		env_add_back(&data->env, node);
		free_argv(tmp);
	}
}

void	remove_env_var(t_env **env, const char *key)
{
	t_env	*curr;
	t_env	*prev;

	curr = *env;
	prev = NULL;
	while (curr)
	{
		if (strcmp(curr->key, key) == 0)
		{
			if (prev)
				prev->next = curr->next;
			else
				*env = curr->next;
			free(curr->key);
			free(curr->value);
			free(curr);
			return ;
		}
		prev = curr;
		curr = curr->next;
	}
}
