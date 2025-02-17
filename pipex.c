
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
    if (!env[i])
        return (NULL);
    array = ft_split(env[i] + 5, ':');
    if (!array)
        return (NULL);
    i = 0;
    while (array[i])
    {
        cmd_slash = ft_strjoin_1(array[i], "/");
        join = ft_strjoin_1(cmd_slash, cmd);
        free(cmd_slash);
        if (access(join, X_OK | F_OK) == 0)
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
        path = check_path(cmd1[0], env);
        if (path == NULL)
        {
            printf("command not found");
            free_split(cmd1);
            return 1;
        }
        execve(path, cmd1, env);
        perror("Execve child fail");
        free(path);
        free_split(cmd1);
        return (1);
}
int parent_case(int *fds, char **av, char **env)
{
    char *path;
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
    path = check_path(cmd2[0], env);
    if (path == NULL)
    {
        free_split(cmd2);
        printf("command not found");
        return 1;
    }
    execve(path, cmd2, env);
    perror("Execve parent fail");
    free(path);
    free_split(cmd2);
    return 1;
}
int main(int ac, char **av, char **env)
{
    if (ac != 5)
    {
        printf("you need to enter all the params needed.\n");
        return (1);
    }
   int fds[2];
   if (pipe(fds) == -1)
        perror("pipe");
   int id = fork();
   if (id == -1)
   {
        perror("fork");
        return (1);
   }
   if(id == 0)
    child_case(fds, av, env);
   else
        parent_case(fds, av, env);
    return (0);
}
