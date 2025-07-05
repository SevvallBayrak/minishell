#include "builtin.h"

int	ft_unset(char **argv, t_data *data)
{
	int	i = 1;

	while (argv[i])
	{
		if (!is_valid_key(argv[i]))
		{
			write(2, "unset: not a valid identifier\n", 31);
		}
		else
		{
			remove_env_var(&data->env, argv[i]);
		}
		i++;
	}
	return (0);
}
