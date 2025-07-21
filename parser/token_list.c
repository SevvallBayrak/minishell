#include "minishell.h"
#include "utils.h"
#include "parser.h"

t_token *create_token(char *value, int type, int quote)
{
    t_token *new = ft_calloc(sizeof(t_token), sizeof(t_token));
    if (!new)
        return (NULL);
    new->value = ft_strdup(value);  // value'nun kopyasını al
    new->type = type;
	new->quote_type = quote;
    new->next = NULL;
    return (new);
}

// add_token fonksiyonu (muhtemelen lexer'a yakın bir yerde bulunur)
// İlk parametre: t_token **head
// İkinci parametre: char *value
// Üçüncü parametre: t_token_type type
// DÖRDÜNCÜ PARAMETRE EKLENİYOR: int q_type (yani senin quote_type alanın)
void add_token(t_token **head, char *value, t_token_type type, int q_type) // int q_type eklendi
{
    t_token *new_token;

    new_token = malloc(sizeof(t_token));
    if (!new_token)
    {
        // Bellek hatası yönetimi
        return; 
    }
    new_token->value = ft_strdup(value);
    new_token->type = type;
    new_token->quote_type = q_type; // <-- Buraya q_type değerini ata!
    new_token->next = NULL;
    // Eğer prev alanı varsa onu da başlatmalısın (new_token->prev = NULL; gibi).
    // Senin t_token yapında prev alanı yok, bu yüzden sadece next'i başlatman yeterli.

    if (!*head)
    {
        *head = new_token;
    }
    else
    {
        t_token *last = *head;
        while (last->next)
            last = last->next;
        last->next = new_token;
    }
}

void	free_token_list(t_token *list)
{
	t_token	*temp;

	while (list)
	{
		temp = list->next;
		free(list->value);
		free(list);
		list = temp;
	}
}
void	free_str_array(char **arr)
{
	int i = 0;

	if (!arr)
		return;
	while (arr[i])
	{
		free(arr[i]);
		i++;
	}
	free(arr);
}
t_cmd	*init_new_cmd(void)
{
	t_cmd *cmd = malloc(sizeof(t_cmd));
	if (!cmd)
		return (NULL);
	ft_bzero(cmd, sizeof(t_cmd));
	cmd->infile = NULL;
	cmd->outfile = NULL;
	cmd->heredoc_delim = NULL;
	return (cmd);
}
