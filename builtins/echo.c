/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sbayrak <sbayrak@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/25 05:08:36 by sbayrak           #+#    #+#             */
/*   Updated: 2025/07/27 18:02:56 by sbayrak          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int ft_echo(char **argv)
{
    int i;
    int newline;

    i = 1;       // Argümanları program adını atlayarak 1. indexten başlat
    newline = 1; // Varsayılan olarak sona yeni satır karakteri ekle

    // Argümanlar arasında "-n", "-nn", "-nnnn" gibi bayrakları ara
    // Bu döngü, bayrak olmayan bir argüman bulana kadar devam eder.
    while (argv[i] && argv[i][0] == '-' && argv[i][1] == 'n')
    {
        int j = 1;
        // Argümanın sadece 'n' karakterlerinden oluşup oluşmadığını kontrol et
        while (argv[i][j] == 'n')
        {
            j++;
        }

        // Eğer string'in sonuna geldiysek (yani '-n' dışında başka karakter yoksa)
        // bu geçerli bir bayraktır.
        if (argv[i][j] == '\0')
        {
            newline = 0; // Yeni satır karakterini iptal et
            i++;         // Bir sonraki argümana geç
        }
        else
        {
            // "-nhello" gibi bir durum varsa, bu bir bayrak değildir.
            // Bayrak arama döngüsünden çık.
            break;
        }
    }

    // Geriye kalan argümanları ekrana yazdır
    while (argv[i])
    {
        // HATA DÜZELTİLDİ: write fonksiyonuna string'in başlangıç adresi (argv[i]) verilir.
        write(STDOUT_FILENO, argv[i], ft_strlen(argv[i]));

        // Eğer bu son argüman değilse, araya bir boşluk koy
        if (argv[i + 1])
            write(STDOUT_FILENO, " ", 1);

        i++; // Bir sonraki argümana geç
    }

    // Eğer -n bayrağı kullanılmadıysa, en sona yeni satır karakteri ekle
    if (newline)
        write(STDOUT_FILENO, "\n", 1);

    return (0);
}

