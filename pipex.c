
#include "pipex.h"
#include "get_next_line/get_next_line.h"

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
int	ft_strncmp(const char *s1, const char *s2, size_t n)
{
	size_t	i;

	i = 0;
	if (n == 0)
		return (0);
	while (i < n)
	{
		if (s1[i] == '\0' || s2[i] == '\0')
			return ((unsigned char)s1[i] - (unsigned char)s2[i]);
		if (s1[i] != s2[i])
			return ((unsigned char)s1[i] - (unsigned char)s2[i]);
		i++;
	}
	return (0);
}


char *check_path(char *cmd, char **env)
{
    int i = 0;
    char **array;
    char *join;
    char *cmd_slash;

    while (env[i] && ft_strncmp("PATH=", env[i], 5) != 0)
        i++;
    if (env[i] == NULL)
        return (NULL);
    array = ft_split(env[i] + 5, ':');
    if (array == NULL)
        return (NULL);
    i = 0;
    while (array[i])
    {
        cmd_slash = ft_strjoin_1(array[i], "/");
        join = ft_strjoin_1(cmd_slash, cmd);
        free(cmd_slash);
        if (access(join, F_OK | X_OK) == 0)
        {
            free_split(array);
            return (join);
        }
        free(join);
        i++;
    }
    free_split(array);
    return (NULL);
}


int    child_case(int *fds, char **av, char **env)
{
    char *path;
    int fd;

    fd = open(av[1], O_RDONLY);
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
    path = check_path(cmd1[0], env);
    if (path == NULL)
    {
        perror("command not found");
        free_split(cmd1);
        return 1;
    }
    execve(path, cmd1, env);
    perror("Execve child fail");
    free(path);
    free_split(cmd1);
    exit(1);
}
int second_child(int *fds, char **av, char **env)
{
    char *path;
    int fd_out = open(av[4], O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if(fd_out == -1)
    {
        perror("Error opening outfile\n");
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
    path = check_path(cmd2[0], env);
    if (path == NULL)
    {
        free_split(cmd2);
        perror("command not found");
        return 1;
    }
    execve(path, cmd2, env);
    perror("Execve child fail");
    free(path);
    free_split(cmd2);
    exit(1);
}

int main(int ac, char **av, char **env)
{
   int fds[2];
    int pid1;
    int pid2;
    if (ac < 5)
    {
        perror("you need to enter all the params needed.\n");
        return (1);
    }
   if (ft_strcmp("here_doc", av[1]) == 0)
   {
        if (here_doc(ac, av, env) == 1)
            return 1;
   }
   else
   {
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
            child_case(fds, av, env);
        pid2 = fork();
        if (pid2 == -1)
        {
            perror("fork");
            return (1);
        }
        if (pid2 == 0)
            second_child(fds, av, env);
        close(fds[0]);
        close(fds[1]);
        waitpid(pid1, NULL, 0);
        waitpid(pid2, NULL, 0);
   }
    return (0);
}
