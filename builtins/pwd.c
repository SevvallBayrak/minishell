
#include "builtin.h"

int ft_pwd(t_data *data)
{
    char cwd[1024];

    (void)data;

    if (!getcwd(cwd, sizeof(cwd)))
    {
        perror("pwd");
        return 1;  // getcwd başarısız
    }

    if (write(1, cwd, strlen(cwd)) == -1 || write(1, "\n", 1) == -1)
    {
        perror("pwd: write error");
        return 1;  // write hatası
    }

    return 0;  // başarılı
}
