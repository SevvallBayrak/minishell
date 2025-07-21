// #include "../includes/minishell.h"

// void init_env(t_data *data, char **env)
// {
// 	char **tmp;
// 	int i;

// 	i = -1;
// 	while(env[++i])
// 	{
// 		tmp = ft_split(env[i], '=');
// 		ft_lstaddback_env(ft_lstnew_env(tmp[1], tmp[2]));
// 	}
// }
// int main(int argc, char **argv, char **env)
// {
	
// }

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


void	init_env(t_data *data, char **envp)
{
	int		i = -1;
	char	**tmp;
	t_env	*node;
	int j;

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
	    {    node->value = ft_strdup(tmp[1]);
			j = 2;
			  while (tmp[j])
            {
                // Önceki değeri serbest bırakarak yeni birleştirilmiş stringi oluştur
                char *temp_value = ft_strjoin(node->value, "="); // Araya '=' ekle
                if (!temp_value) { // ft_strjoin hatası
                    free(node->value); free(node->key); free(node); ft_free_split(tmp); return;
                }
                free(node->value); // Eski node->value'yu serbest bırak
                node->value = ft_strjoin(temp_value, tmp[j]); // Yeni parçayı ekle
                free(temp_value); // Ara stringi serbest bırak
                if (!node->value) { // ft_strjoin hatası
                    free(node->key); free(node); ft_free_split(tmp); return;
                }
                j++;
            }
		}
        else
	    {    node->value = NULL;}
		node->next = NULL;
		env_add_back(&data->env, node);
		ft_free_split(tmp);
	}
}
