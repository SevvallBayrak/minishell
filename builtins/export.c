/*
🔧 Fonksiyon Olarak Yapılacaklar

argv'yi dolaş: export sonrası gelen her argümanı kontrol et.
Eşittir (=) var mı kontrol et.
= varsa:
key ve value'yu ayır (split).
geçerli key mi kontrol et.
update_env_var(data, key, value) ile güncelle.
= yoksa:
key geçerliyse value = NULL olarak ekle.
Hiç argüman yoksa: tüm environment'ı alfabetik sırayla yazdır.
*/

#include "builtin.h"

int	ft_export(char **argv, t_data *data)
{
	int	i = 1;

	while (argv[i])
	{
		process_export_arg(argv[i], data);
		i++;
	}
	return (0);
}

void	process_export_arg(char *arg, t_data *data)
{
	char	*equal_sign;
	char	*key;
	char	*value;

	equal_sign = ft_strchr(arg, '=');
	if (equal_sign)
	{
		*equal_sign = '\0';
		key = arg;
		value = equal_sign + 1;
	}
	else
	{
		key = arg;
		value = NULL;
	}
	if (!is_valid_key(key))
	{
		write(2, "export: not a valid identifier\n", 31);
		return;
	}
	if (key && value)
		update_env_var(data, key, value);
}

int is_valid_key(const char *key)
{
    int i = 0;

    if (!key || (!ft_isalpha(key[0]) && key[0] != '_'))
        return 0;

    while (key[i])
    {
        if (!ft_isalnum(key[i]) && key[i] != '_')
            return 0;
        i++;
    }
    return 1;
}
