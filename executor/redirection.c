#include "minishell.h"

int	redirect_in(t_cmd *cmd, t_data *data)
{
	int	fd;

	if (!cmd->infile)
		return (0);
	fd = open(cmd->infile, O_RDONLY);
	if (fd < 0)
	{
		data->exit_status = 1;
		perror(cmd->infile);
		return (1); // hata
	}
	if (dup2(fd, STDIN_FILENO) == -1)
	{    
		data->exit_status = 1;
        perror("dup2");
        close(fd);
        return (1);
    }
	close(fd);
	return (0);
}
int	redirect_out(t_cmd *cmd, t_data *data)
{
	int	fd;

	if (!cmd->outfile)
		return (0);
	if (cmd->append)
		fd = open(cmd->outfile, O_WRONLY | O_CREAT | O_APPEND, 0644);
	else
		fd = open(cmd->outfile, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd < 0)
	{
		perror(cmd->outfile);
		return (1); // hata
	}
	if (dup2(fd, STDOUT_FILENO) == -1)
    {    
		data->exit_status = 1;
        perror("dup2");
        close(fd);
        return (1);
    }
	close(fd);
	return (0);
}
