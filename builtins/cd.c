/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sbayrak <sbayrak@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/25 05:04:08 by sbayrak           #+#    #+#             */
/*   Updated: 2025/07/26 23:40:28 by sbayrak          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	update_pwd_vars(t_data *data, const char *oldpwd)
{
	char	cwd[1024];

	if (oldpwd)
		update_env_var(data, "OLDPWD", oldpwd);
	if (getcwd(cwd, sizeof(cwd)))
		update_env_var(data, "PWD", cwd);
}

static char	*handle_cd_home(t_data *data)
{
	char	*path;

	path = ft_strdup(get_env_value(data->env, "HOME"));
	if (!path)
	{
		write(2, "cd: HOME not set\n", 17);
		return (NULL);
	}
	return (path);
}

static char	*handle_cd_dash(t_data *data)
{
	char	*path;

	path = ft_strdup(get_env_value(data->env, "OLDPWD"));
	if (!path)
	{
		write(2, "cd: OLDPWD not set\n", 19);
		return (NULL);
	}
	if (write(1, path, strlen(path)) == -1
		|| write(1, "\n", 1) == -1)
	{
		perror("cd: write error");
		return (NULL);
	}
	return (path);
}

static char	*get_target_path(char **argv, t_data *data)
{
	if (!argv[1])
		return (handle_cd_home(data));
	if (strcmp(argv[1], "-") == 0)
		return (handle_cd_dash(data));
	return (argv[1]);
}

int	ft_cd(char **argv, t_data *data)
{
	char	*path;
	char	*oldpwd;
	int		ret;

	ret = 0;
	oldpwd = ft_strdup(get_env_value(data->env, "PWD"));
	path = get_target_path(argv, data);
	if (!path)
		return (free(oldpwd), 1);
	if (argv[1] && argv[2])
	{
		write(2, "cd: too many arguments\n", 24);
		ret = 1;
	}
	if (!ret && chdir(path) != 0)
	{
		perror("cd");
		ret = 1;
	}
	if (!ret)
		update_pwd_vars(data, oldpwd);
	if (path != argv[1])
		free(path);
	free(oldpwd);
	return (ret);
}
