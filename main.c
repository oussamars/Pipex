#include "pipex.h"
#include "get_next_line/get_next_line.h"

int main(int ac, char **av)
{
    (void)ac;
    char *line;
    char *join;
    int fd = open(av[1], O_RDONLY);

    if (fd == -1)
    {
        printf("Error \n");
        return (1);
    }
    line = get_next_line(fd);
    join = ft_strdup_1(line);
    while (line != NULL)
    {
        printf("1\n");
        join = ft_strjoin_1(join, line);
        line = get_next_line(fd);
    }
    printf("%s\n", join);
}