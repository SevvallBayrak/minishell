#include "builtin.h"

static void	update_pwd_vars(t_data *data, const char *oldpwd)
{
	char	cwd[1024];

	if (oldpwd)
		update_env_var(data, "OLDPWD", oldpwd);
	if (getcwd(cwd, sizeof(cwd)))
		update_env_var(data, "PWD", cwd);
}

static char	*get_target_path(char **argv, t_data *data)
{
	char	*path;

	if (!argv[1])
	{
		path = get_env_value(data->env, "HOME");
		if (!path)
		{
			write(2, "cd: HOME not set\n", 18);
			return (NULL);
		}
	}
	else if (strcmp(argv[1], "-") == 0)
	{
		path = get_env_value(data->env, "OLDPWD");
		if (!path)
		{
			write(2, "cd: OLDPWD not set\n", 20);
			return (NULL);
		}
		if (write(1, path, strlen(path)) == -1 || write(1, "\n", 1) == -1)
		{
			perror("cd: write error");
			return (NULL);
		}
	}
	else
		path = argv[1];
	return (path);
}

int	ft_cd(char **argv, t_data *data)
{
	char	*path;
	char	*oldpwd;

	oldpwd = get_env_value(data->env, "PWD");
	path = get_target_path(argv, data);
	if (!path)
		return (1); // Hedef path yoksa, hata

	if (chdir(path) != 0)
	{
		perror("cd");
		return (1); // chdir başarısız
	}
	if (argv[2])///TO MANY ARGUMAN HATASI!!!!
	{
		write(2, "cd: too many arguments\n", 24);
		return (1); // Çok fazla argüman
	}
	update_pwd_vars(data, oldpwd);
	return (0); // Başarılı
}
