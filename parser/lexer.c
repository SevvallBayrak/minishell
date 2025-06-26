#include "parser.h"

void lexer(const char *line)
{
    int i = 0;

    while (line[i])
    {
        // Her karakteri sırayla işleyebilirsin
        // Örneğin boşluk atla:
        if (line[i] == ' ')
        {
            i++;
            continue;
        }
        while (line[i])
        {
        }
        // Diğer durumlar için şimdilik debug bastır:
        printf("karakter: %c\n", line[i]);
        i++;
    }
}

#include "parser.h"
#include <stdlib.h>
#include <string.h>

t_token	*new_token(t_token_type type, const char *value)
{
    t_token	*token;

    token = malloc(sizeof(t_token));
    if (!token)
        return (NULL);
    token->type = type;
    token->value = strdup(value); // value'yu kopyalıyoruz
    token->next = NULL;
    return (token);
}