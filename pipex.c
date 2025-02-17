
#include "pipex.h"
#include "get_next_line/get_next_line.h"

char	**ft_split(char const *s, char c);
void free_split(char **split)
{
    int i = 0;
    while (split[i])
    {
        free(split[i]);
        i++;
    }
    free(split);
}

int    child_case(int *fds, char **av)
{
    int fd = open(av[1], O_RDONLY);
        if (fd == -1)
        {
            perror("open infile");
            return (1);
        }
        close(fds[0]);
        char **cmd1 = ft_split(av[2], ' ');
        if (cmd1 == NULL || cmd1[0] == NULL)
        {
            perror("Invalid command or command split failed");
            return (1);
        }
        dup2(fd, 0);
        dup2(fds[1], 1);
        close(fds[1]);
        execvp(cmd1[0], cmd1);
        perror("Execvp child fail");
        free_split(cmd1);
        return (1);
}
int parent_case(int *fds, char **av)
{
    int fd_out = open(av[4], O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if(fd_out == -1)
    {
        printf("Error opening outfile\n");
        return (1);
    }
    wait(NULL);
    close(fds[1]);
    char **cmd2 = ft_split(av[3], ' ');
    if (cmd2 == NULL || cmd2[0] == NULL)
    {
        perror("Invalid command or command split failed");
        return (1);
    }
    dup2(fds[0], 0);
    dup2(fd_out, 1);
    execvp(cmd2[0], cmd2);
    perror("Execvp parent fail");
    free_split(cmd2);
    return 1;
}
int main(int ac, char **av)
{
    if (ac != 5)
    {
        printf(("you need to enter all the params needed.\n"));
        return (1);
    }
   int fds[2];
   pipe(fds);
   if (pipe(fds) == -1)
        perror("pipe");
   int id = fork();
   if (id == -1)
        perror("fork");
   if(id == 0)
    child_case(fds, av);
   else
        parent_case(fds, av);
    return (0);
}
