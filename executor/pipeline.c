
#include "minishell.h"
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <stdio.h>

int has_pipe(t_cmd *cmds)
{
    t_cmd *current = cmds;
    
	while (current)
	{
		if (current->next)
			return (1);
		current = current->next;
	}
	return (0);
}
void pipe_child_exec(t_cmd *cmd, int in_fd, int out_fd, t_data *data)
{
	if (in_fd != STDIN_FILENO)
	{
		dup2(in_fd, STDIN_FILENO);
		close(in_fd);
	}
	if (out_fd != STDOUT_FILENO)
	{
		dup2(out_fd, STDOUT_FILENO);
		close(out_fd);
	}
	if (cmd->is_heredoc)
	{
		int hd_fd = handle_heredoc(cmd, data);
		if (hd_fd == -1)
			exit(1);
		if (dup2(hd_fd, STDIN_FILENO) == -1)
		{
			perror("dup2");
			close(hd_fd);
			exit(1);
		}
		close(hd_fd);
	}
	if (redirect_in(cmd) || redirect_out(cmd))
		exit(1);

	if (is_builtin(cmd->argv[0]))
		exit(exec_builtin(cmd, data));
	else
		exit(execute_command(cmd, cmd->argv, data));
}
void close_pipe_ends(int *pipefd)	
{
	close(pipefd[0]);
	close(pipefd[1]);
}
int run_pipeline(t_cmd *cmds, t_data *data)
{
    int pipefd[2];
    int in_fd = STDIN_FILENO;
    pid_t pid;
    t_cmd *current_cmd = cmds;
    int num_cmds = 0;
    pid_t *pids = NULL; // Tüm çocuk PID'lerini saklamak için dinamik dizi

    // İlk geçiş: Komut sayısını bul
    t_cmd *temp_cmd = cmds;
    while (temp_cmd) {
        num_cmds++;
        temp_cmd = temp_cmd->next;
    }

    if (num_cmds > 0) {
        pids = malloc(sizeof(pid_t) * num_cmds);
        if (!pids) return (perror("malloc"), 1);
    } else {
        return (0); // Çalıştırılacak komut yok
    }

    int i = 0; // PID dizisi indeksi
    while (current_cmd)
    {
        // Eğer son komut değilse, pipe oluştur
        if (current_cmd->next)
        {
            if (pipe(pipefd) == -1)
            {
                // Pipe hatası: Önceki in_fd'yi kapat, daha önce fork'lanmış çocukları bekle ve free et
                if (in_fd != STDIN_FILENO) close(in_fd);
                for (int k = 0; k < i; k++) waitpid(pids[k], NULL, 0); 
                free(pids);
                return (perror("pipe"), 1);
            }
        }

        pid = fork();
        if (pid == -1) // Fork hatası
        {
            // Fork hatası: Pipe uçlarını kapat, önceki in_fd'yi kapat, daha önce fork'lanmış çocukları bekle ve free et
            if (current_cmd->next) { close(pipefd[0]); close(pipefd[1]); }
            if (in_fd != STDIN_FILENO) close(in_fd);
            for (int k = 0; k < i; k++) waitpid(pids[k], NULL, 0); 
            free(pids);
            return (perror("fork"), 1);
        }
        else if (pid == 0) // Çocuk süreç
        {
            int out_fd = current_cmd->next ? pipefd[1] : STDOUT_FILENO;

            // Çocuk süreçte kullanılmayan pipe uçlarını kapatmak kritik!
            if (current_cmd->next) { // Eğer pipe oluşturulduysa
                close(pipefd[0]); // Çocuk bu pipe'ın okuma ucunu kullanmaz
            }
            // Diğer pipe uçları (in_fd, out_fd) pipe_child_exec içinde dup2 edildikten sonra kapatılacak.

            pipe_child_exec(current_cmd, in_fd, out_fd, data);
            // pipe_child_exec exit() çağırır, bu yüzden buraya gelmemeli.
        }
        else // Ebeveyn süreç
        {
            pids[i] = pid; // Çocuk PID'ini kaydet

            if (in_fd != STDIN_FILENO)
                close(in_fd); // Önceki pipe'ın okuma ucunu kapat (eğer varsa)

            if (current_cmd->next) // Eğer son komut değilse
            {
                close(pipefd[1]); // Mevcut pipe'ın yazma ucunu kapat
                in_fd = pipefd[0]; // Sonraki komut için okuma ucunu ayarla
            }
            else // Son komutsa
            {
                 // Eğer son komutsa ve önceki bir pipe'tan gelen bir in_fd varsa, onu kapat.
                 if (in_fd != STDIN_FILENO)
                    close(in_fd);
            }
        }
        current_cmd = current_cmd->next;
        i++;
    }

    // TÜM ÇOCUK SÜREÇLERİ BEKLE
    int last_cmd_status = 0;
    for (int j = 0; j < num_cmds; j++)
    {
        int status;
        waitpid(pids[j], &status, 0);
        // Bash, pipe hattının son komutunun çıkış durumunu döndürür.
        if (j == num_cmds - 1) { // Eğer bu, pipe hattındaki son komutsa
            if (WIFEXITED(status))
                last_cmd_status = WEXITSTATUS(status);
            else if (WIFSIGNALED(status))
                last_cmd_status = 128 + WTERMSIG(status); // Sinyal ile sonlanma durumu
        }
    }
    data->exit_status = last_cmd_status; // Shell'in exit durumunu güncelle
    free(pids); // PID dizisini serbest bırak

    return (0);
}

