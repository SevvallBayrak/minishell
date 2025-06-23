#include "minishell.h"

char *expand_variable(const char *token, t_shell_state *shell)
{
    if (!token || !ft_strchr(token, '$'))
        return ft_strdup(token);
    
    char *result = ft_calloc(1, 1);
    const char *p = token;
    
    while (*p)
    {
        if (*p == '$')
        {
            if (*(p + 1) == '?')
            {
                // $? - exit status expansion
                char buffer[12];
                *buffer = *ft_itoa(shell->last_exit_status);
                printf("%s" , buffer);
                size_t old_len = ft_strlen(result);
                size_t new_len = old_len + ft_strlen(buffer);
                char *tmp = realloc(result, new_len + 1);//!!!!
                if (!tmp) {
                    free(result);
                    return NULL;
                }
                result = tmp;
                strcat(result, buffer);
                p += 2;
            }
            else if (*(p + 1) && (ft_isalnum(*(p + 1)) || *(p + 1) == '_'))
            {
                // Variable name expansion
                p++; // $ karakterini atla
                const char *start = p;
                while (*p && (ft_isalnum(*p) || *p == '_'))
                    p++;
                
                char *var = ft_strndup(start, p - start);
                char *value = getenv(var);
                
                size_t old_len = ft_strlen(result);
                size_t value_len = value ? ft_strlen(value) : 0;
                char *tmp = realloc(result, old_len + value_len + 1);//!!!!!!
                if (!tmp) {
                    free(var);
                    free(result);
                    return NULL;
                }
                result = tmp;
                if (value)
                    strcat(result, value);
                free(var);
            }
            else
            {
                // $ sonrası geçersiz karakter, $ karakterini olduğu gibi ekle
                size_t old_len = ft_strlen(result);
                char *tmp = realloc(result, old_len + 2);//!!!!!!
                if (!tmp) {
                    free(result);
                    return NULL;
                }
                result = tmp;
                result[old_len] = '$';
                result[old_len + 1] = '\0';
                p++;
            }
        }
        else
        {
            // Normal karakter
            size_t old_len = ft_strlen(result);
            char *tmp = realloc(result, old_len + 2);
            if (!tmp) {
                free(result);
                return NULL;
            }
            result = tmp;
            result[old_len] = *p;
            result[old_len + 1] = '\0';
            p++;
        }
    }
    return result;
}

#include <stdlib.h>

char    *ft_strndup(const char *s, size_t n)
{
    char    *dup;
    size_t  i;

    dup = (char *)malloc(n + 1);
    if (!dup)
        return (NULL);
    i = 0;
    while (i < n && s[i])
    {
        dup[i] = s[i];
        i++;
    }
    dup[i] = '\0';
    return (dup);
}
