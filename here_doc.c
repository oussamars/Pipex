#include "pipex.h"
#include "get_next_line/get_next_line.h"


int ft_strcmp(const char *s1, const char *s2)
{
    while (*s1 && (*s1 == *s2))
    {
        s1++;
        s2++;
    }
    return (unsigned char)*s1 - (unsigned char)*s2;
}


int child_case_here_doc(int *fds, char **av, char **env)
{
    char *line;
    char *path;
    char **cmd1;

    close(fds[0]);
    while(1)
    {
        write(1, "heredoc> ", 9);
        line = get_next_line(0);
        if (line == NULL)
            break;
        if (ft_strncmp(line, av[2], ft_strlen(av[2])) == 0
            && line[ft_strlen(av[2])] == '\n')
        {
            free(line);
            return (0);
        }
        write(fds[1], line, ft_strlen(line));
        free(line);
    }
    close(fds[1]);
    cmd1 = ft_split(av[3], ' ');
    if (cmd1 == NULL || cmd1[0] == NULL)
    {
        perror("Invalid command or command split failed");
        return (1);
    }
    dup2(fds[0], 0);
    path = check_path(cmd1[0], env);
    if (path == NULL)
    {
        free_split(cmd1);
        perror("command not found");
        exit(1);
    }
    execve(path, cmd1, env);
    free(path);
    free_split(cmd1);
    exit(1);

}

int second_child_here_doc(int ac, int *fds, char **av, char **env)
{
    char *path;
    int fd_out = open(av[ac -1], O_WRONLY | O_CREAT | O_APPEND, 0644);
    if(fd_out == -1)
    {
        perror("Error opening outfile\n");
        return (1);
    }
    close(fds[1]);
    char **cmd2 = ft_split(av[4], ' ');
    if (cmd2 == NULL || cmd2[0] == NULL)
    {
        perror("Invalid command or command split failed");
        return (1);
    }
    dup2(fds[0], 0);
    dup2(fd_out, 1);
    path = check_path(cmd2[0], env);
    if (path == NULL)
    {
        free_split(cmd2);
        perror("command not found");
        exit(1);
    }
    execve(path, cmd2, env);
    perror("Execve parent fail");
    free(path);
    free_split(cmd2);
    exit(1);
}

int here_doc(int ac, char **av, char **env)
{
    int fds[2];
    int pid1;
    int pid2;

    if (pipe(fds) == -1)
   {
        perror("pipe");
        return (1);
   }
    pid1 = fork();
   if (pid1 == -1)
   {
        perror("fork");
        return (1);
   }
   if (pid1 == 0)
        return child_case_here_doc(fds, av, env);
    pid2 = fork();
    if (pid2 == -1)
    {
        perror("fork");
        return (1);
    }
    if (pid2 == 0)
        return second_child_here_doc(ac, fds, av, env);
    close(fds[0]);
    close(fds[1]);
    waitpid(pid1, NULL, 0);
    waitpid(pid2, NULL, 0);
    return (0);
}
