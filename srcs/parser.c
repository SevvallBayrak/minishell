#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>

bool	check_unclosed_quotes(const char *str)
{
	char	quote = 0;

	while (*str)
	{
		if ((*str == '\'' || *str == '"') && quote == 0)
			quote = *str;
		else if (*str == quote)
			quote = 0;
		str++;
	}
	return (quote != 0);
}

// Özel karakter kontrolü
int is_special(char c) {
    return (c == '|' || c == '<' || c == '>');
}

// Tırnaklı token kopyalama
static char *copy_quoted_token(const char **str) {
    char quote = *(*str)++; // ' veya "
    const char *start = *str;
    int len = 0;

    while (**str && **str != quote) {
        (*str)++;
        len++;
    }

    char *token = strndup(start, len);
    if (**str == quote)
        (*str)++; // Kapanış tırnağını atla
    return token;
}

// Basit split mantığı ile tokenize fonksiyonu
char **tokenize_input(const char *input) {
    // input'u değiştirmemek için kopyasını al
    char *str = strdup(input);
    const char *p = str;
    int capacity = 10;
    char **tokens = malloc(capacity * sizeof(char *));
    int count = 0;

    while (*p) {
        while (isspace(*p))
            p++;
        if (!*p)
            break;

        char *token = NULL;

        if (*p == '\'' || *p == '"') {
            token = copy_quoted_token(&p);
        }
        else if (is_special(*p)) {
            token = malloc(2);
            token[0] = *p;
            token[1] = '\0';
            p++;
        }
        else {
            const char *start = p;
            while (*p && !isspace(*p) && !is_special(*p) && *p != '\'' && *p != '"')
                p++;
            token = strndup(start, p - start);
        }

        if (count >= capacity) {
            capacity *= 2;
            tokens = realloc(tokens, capacity * sizeof(char *));
        }
        tokens[count++] = token;
    }
    tokens[count] = NULL;
    free(str);
    return tokens;
}

