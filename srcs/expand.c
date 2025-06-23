#include "minishell.h"
#include "parser.h"

char *expand_variable(const char *token, t_shell_state *shell)
{
    if (!token || !strchr(token, '$'))
        return strdup(token);
    
    char *result = calloc(1, 1);
    const char *p = token;
    
    while (*p)
    {
        if (*p == '$')
        {
            if (*(p + 1) == '?')
            {
                // $? - exit status expansion
                char buffer[12];
                sprintf(buffer, "%d", shell->last_exit_status);
                size_t old_len = strlen(result);
                size_t new_len = old_len + strlen(buffer);
                char *tmp = realloc(result, new_len + 1);
                if (!tmp) {
                    free(result);
                    return NULL;
                }
                result = tmp;
                strcat(result, buffer);
                p += 2;
            }
            else if (*(p + 1) && (isalnum(*(p + 1)) || *(p + 1) == '_'))
            {
                // Variable name expansion
                p++; // $ karakterini atla
                const char *start = p;
                while (*p && (isalnum(*p) || *p == '_'))
                    p++;
                
                char *var = strndup(start, p - start);
                char *value = getenv(var);
                
                size_t old_len = strlen(result);
                size_t value_len = value ? strlen(value) : 0;
                char *tmp = realloc(result, old_len + value_len + 1);
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
                size_t old_len = strlen(result);
                char *tmp = realloc(result, old_len + 2);
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
            size_t old_len = strlen(result);
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