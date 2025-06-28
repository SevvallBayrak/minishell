// #include "parser.h"

// void lexer(const char *line)
// {
//     int i = 0;

//     while (line[i])
//     {
//         // Her karakteri sırayla işleyebilirsin
//         // Örneğin boşluk atla:
//         if (line[i] == ' ')
//         {
//             i++;
//             continue;
//         }
//         while (line[i])
//         {
//         }
//         // Diğer durumlar için şimdilik debug bastır:
//         printf("karakter: %c\n", line[i]);
//         i++;
//     }
// }

// #include "parser.h"
// #include <stdlib.h>
// #include <string.h>

// t_token	*new_token(t_token_type type, const char *value)
// {
//     t_token	*token;

//     token = malloc(sizeof(t_token));
//     if (!token)
//         return (NULL);
//     token->type = type;
//     token->value = strdup(value); // value'yu kopyalıyoruz
//     token->next = NULL;
//     return (token);
// }


#include "minishell.h"
#include <stdlib.h>
#include <string.h>

t_token *lexer(char *input)
{
    t_token *tokens = NULL;
    int i = 0;

    while (input[i])
    {
        if (isspace(input[i]))
            i++;
        else if (input[i] == '\'' || input[i] == '"')
            i += handle_quote(input, i, input[i]); // quote içinde
        else if (is_operator(input[i]))
            i += handle_redirection(input, &tokens, i);
        else
            i += handle_word(input, &tokens, i);
    }
    return tokens;
}

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
