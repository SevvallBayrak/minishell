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
	load_env_from_envp(data, envp);
	//ensure_pwd_exists(data);
}

void	load_env_from_envp(t_data *data, char **envp)
{
	int		i = -1;
	char	**tmp;
	t_env	*node;

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
	        node->value = ft_strdup(tmp[1]);
        else
	        node->value = NULL;
		node->next = NULL;
		env_add_back(&data->env, node);
		ft_free_split(tmp);
	}
}
/*void	ensure_pwd_exists(t_data *data)
{
	char cwd[1024];

	if (!get_env_value(data->env, "PWD"))
	{
		if (getcwd(cwd, sizeof(cwd)))
			update_env_var(data, "PWD", cwd);
	}
}*/
