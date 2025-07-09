#include "builtin.h"

int	ft_unset(char **argv, t_data *data)
{
	int	i = 1;
	int	has_error = 0;

	while (argv[i])
	{
		if (!is_valid_key(argv[i]))
		{
			write(2, "unset: not a valid identifier\n", 31);
			has_error = 1;
		}
		else
		{
			remove_env_var(&data->env, argv[i]);
		}
		i++;
	}
	return (has_error);
}
