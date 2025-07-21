#include "minishell.h"
#include "utils.h"
#include "parser.h"
#include <unistd.h> // write için

// utils.c veya libft'nizde
char *append_char_to_str(char *s, char c)
{
    char *new_s;
    size_t len;

    if (!s)
    {
        new_s = malloc(sizeof(char) * 2); // 1 karakter + null
        if (!new_s) return (NULL);
        new_s[0] = c;
        new_s[1] = '\0';
        return (new_s);
    }
    len = ft_strlen(s);
    new_s = malloc(sizeof(char) * (len + 2)); // Mevcut + 1 karakter + null
    if (!new_s) return (NULL);
    strcpy(new_s, s); // Mevcut içeriği kopyala
    new_s[len] = c; // Yeni karakteri ekle
    new_s[len + 1] = '\0';
    free(s); // Eski stringi serbest bırak
    return (new_s);
}

// lexer.c
t_token *lexer(char *input, t_data *data)
{
    t_token *tokens = NULL;
    int i = 0;
    char *current_lex_word = NULL;
    int start;
    int current_token_quote_type = 0; // <-- Sadece bu değişkeni kullanacağız (0: NONE, 1: SINGLE, 2: DOUBLE)

    while (input[i])
    {
        // Boşluk görürsek
        if (ft_isspace(input[i]))
        {
            if (current_lex_word) // Biriken bir kelime varsa, token olarak ekle
            {
                add_token(&tokens, current_lex_word, T_WORD, current_token_quote_type);
                free(current_lex_word);
            }
            current_lex_word = NULL;
            current_token_quote_type = 0; // Token eklendi, tipi sıfırla
            i++;
            continue;
        }

        // Operatörler veya pipe
        if (is_operator(input[i]))
        {
            if (current_lex_word) // Biriken bir kelime varsa, önce onu ekle
            {
                add_token(&tokens, current_lex_word, T_WORD, current_token_quote_type);
                free(current_lex_word);
            }
            current_lex_word = NULL;
            current_token_quote_type = 0; // Token eklendi, tipi sıfırla
            int step = handle_redirection(input, &tokens, i, data);
            if (step == 0)
            {
                free_token_list(tokens);
                if (current_lex_word) free(current_lex_word);
                return (NULL);
            }
            i += step;
            continue;
        }

        // Normal kelime karakterleri veya tırnaklı kelimeler
        start = i; // Kelime toplama segmentinin başlangıcı
        
        // Bu iç döngü kelimenin sonuna kadar veya bir durdurucuya kadar devam eder
        while (input[i] && !ft_isspace(input[i]) && !is_operator(input[i]))
        {
            if (input[i] == '\'' || input[i] == '"') // Tırnak başlangıcı
            {
                char quote_char = input[i];
                
                // Eğer bu, genel token için belirlenen ilk tırnak tipi ise kaydet.
                // Örneğin: 'abc' için 1. "def" için 2. a'b'c için 0 (varsayılan)
                // Bu kritik nokta: eğer zaten bir tırnak tipi belirlenmişse (örn. "a" tek tırnakla başladı)
                // ve sonra yeni bir tırnak türü (örn. 'b' çift tırnakla devam etti) geliyorsa
                // burada bash'in karmaşık kurallarına girmemiz gerekir.
                // Ancak basit testler için ilk tırnak türünü genel tür olarak kabul edebiliriz.
                if (current_token_quote_type == 0) // Henüz bir tırnak tipi belirlenmediyse
                    current_token_quote_type = odd_or_double_quote(quote_char); // Tırnak tipini ata

                i++; // Açılış tırnağını atla
                while (input[i] && input[i] != quote_char)
                {
                    current_lex_word = append_char_to_str(current_lex_word, input[i]);
                    i++;
                }
                if (!input[i]) // Kapanış tırnağı yoksa hata
                {
                    write(2, "minishell: syntax error: unclosed quote\n", 40);
                    data->exit_status = 258;
                    free_token_list(tokens);
                    if (current_lex_word) free(current_lex_word);
                    return (NULL);
                }
                i++; // Kapanış tırnağını atla
            }
            else // Normal kelime karakteri
            {
                current_lex_word = append_char_to_str(current_lex_word, input[i]);
                i++;
            }
        }

        // Kelime toplama döngüsü bittiğinde
        if (current_lex_word && i > start)
        {
            // Eğer kelimeyi ekliyorsak, belirlenen tırnak tipini kullan.
            // Eğer hiç tırnak görmediysek current_token_quote_type 0 kalacak.
            add_token(&tokens, current_lex_word, T_WORD, current_token_quote_type);
            free(current_lex_word);
            current_lex_word = NULL;
            current_token_quote_type = 0; // Token eklendi, tipi sıfırla
        }
    }

    // Döngü bittikten sonra kalan bir kelime varsa ekle
    if (current_lex_word)
    {
        add_token(&tokens, current_lex_word, T_WORD, current_token_quote_type);
        free(current_lex_word);
    }
    return tokens;
}