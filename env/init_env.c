#include "minishell.h"

void	ft_free_split(char **split)
{
	int	i;

	if (!split)
		return ;
	i = 0;
	while (split[i])
	{
		free(split[i]);
		i++;
	}
	free(split);
}

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

void freetmp_and_join (t_env *node, char **tmp, int j)
{
 	// Önceki değeri serbest bırakarak yeni birleştirilmiş stringi oluştur
    char *temp_value;
	temp_value = ft_strjoin(node->value, "="); // Araya '=' ekle
    free(node->value); // Eski node->value'yu serbest bırak
    node->value = ft_strjoin(temp_value, tmp[j]); // Yeni parçayı ekle
    free(temp_value); // Ara stringi serbest bırak
}

void node_value(char **tmp, t_env *node)
{
	int 	j;

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
			continue;
		node = malloc(sizeof(t_env));
		if (!node)
		{
			ft_free_split(tmp);
			continue;
		}
		node->key = ft_strdup(tmp[0]);
		if (tmp[1])
			node_value(tmp, node);
        else
			node->value = NULL;
		node->next = NULL;
		env_add_back(&data->env, node);
		ft_free_split(tmp);
	}
}
