#include "minishell.h"
#include "utils.h"
#include "parser.h"

t_token *create_token(char *value, int type)
{
    t_token *new = malloc(sizeof(t_token));
    if (!new)
        return (NULL);
    new->value = strdup(value);  // value'nun kopyasını al
    new->type = type;
    new->next = NULL;
    return (new);
}

void add_token(t_token **list, char *value, int type)
{
    t_token *new = create_token(value, type);
    t_token *temp;

    if (!new)
        return ;
    if (*list == NULL)
    {
        *list = new;
        return ;
    }
    temp = *list;
    while (temp->next)
        temp = temp->next;
    temp->next = new;
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