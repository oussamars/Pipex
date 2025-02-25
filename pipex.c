/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oboussel <oboussel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/23 15:06:36 by oboussel          #+#    #+#             */
/*   Updated: 2025/02/25 09:25:39 by oboussel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

char	*check_path(char *cmd, char **env)
{
	int		i;
	char	**paths;
	char	*path;
	char	*cmd_path;

	if (cmd[0] == '/' || cmd[0] == '.')
		return (slash_cmd(cmd));
	i = 0;
	while (env[i] && ft_strncmp("PATH=", env[i], 5) != 0)
		i++;
	if (!env[i])
		return (NULL);
	paths = ft_split(env[i] + 5, ':');
	i = 0;
	while (paths[i])
	{
		path = ft_strjoin(paths[i++], "/");
		cmd_path = ft_strjoin(path, cmd);
		free(path);
		if (access(cmd_path, F_OK | X_OK) == 0)
			return (free_split(paths), cmd_path);
		free(cmd_path);
	}
	return (free_split(paths), NULL);
}

void	execute_command(char **cmd, char **env)
{
	char	*path;

	path = check_path(cmd[0], env);
	if (!path)
	{
		ft_putstr_fd("zsh: command not found: ", 2);
		ft_putendl_fd(cmd[0], 2);
		free_split(cmd);
		exit(127);
	}
	execve(path, cmd, env);
	perror("execve");
	free(path);
	free_split(cmd);
	exit(1);
}

void	handle_child(int i, int fds[2], char **av, char **env)
{
	int		fd;
	char	**cmd;

	if (i == 0)
	{
		fd = open(av[1], O_RDONLY);
		if (fd == -1)
			error_child(av[1]);
		dup2(fd, 0);
		dup2(fds[1], 1);
	}
	else
	{
		fd = open(av[4], O_WRONLY | O_CREAT | O_TRUNC, 0644);
		if (fd == -1)
			error_child(av[4]);
		dup2(fds[0], 0);
		dup2(fd, 1);
	}
	close(fds[0]);
	close(fds[1]);
	cmd = ft_split(av[i + 2], ' ');
	if (!cmd || !cmd[0])
		permission_case(cmd);
	execute_command(cmd, env);
}

int	pipex(int ac, char **av, char **env)
{
	int	fds[2];
	int	pid1;
	int	pid2;
	int	status;

	(void)ac;
	if (pipe(fds) == -1)
		return (perror("pipe"), 1);
	pid1 = fork();
	if (pid1 == -1)
		return (perror("fork"), 1);
	if (pid1 == 0)
		handle_child(0, fds, av, env);
	pid2 = fork();
	if (pid2 == -1)
		return (perror("fork"), 1);
	if (pid2 == 0)
		handle_child(1, fds, av, env);
	close(fds[0]);
	close(fds[1]);
	waitpid(pid1, &status, 0);
	waitpid(pid2, &status, 0);
	if (WIFEXITED(status))
		return (WEXITSTATUS(status));
	return (0);
}

int	main(int ac, char **av, char **env)
{
	if (ac != 5)
	{
		ft_putstr_fd("Enter all the arguments\n", 2);
		return (1);
	}
	return (pipex(ac, av, env));
}
