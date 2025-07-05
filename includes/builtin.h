#ifndef BUILTIN_H
# define BUILTIN_H

# include "minishell.h"

int is_builtin(const char *cmd);
int exec_builtin(t_cmd *cmd, t_data *data);

int ft_cd(char **argv, t_data *data);
int ft_echo(char **argv);
int ft_pwd(t_data *data);
int ft_env(t_data *data);                        // ✅ DÜZELTİLDİ
int ft_export(char **argv, t_data *data);        // ✅ DÜZELTİLDİ
int ft_unset(char **argv, t_data *data);         // ✅ DÜZELTİLDİ
int ft_exit(char **argv, t_data *data);
int is_valid_key(const char *key);
void	process_export_arg(char *arg, t_data *data);

#endif