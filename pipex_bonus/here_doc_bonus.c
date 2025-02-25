/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   here_doc_bonus.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oboussel <oboussel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/23 15:07:52 by oboussel          #+#    #+#             */
/*   Updated: 2025/02/25 11:46:24 by oboussel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex_bonus.h"

void	setup_heredoc_child(int i, t_pipex *pipex)
{
	int	fd;

	dup2(pipex->prev_pipe, 0);
	if (i != pipex->argc - 5)
		dup2(pipex->fds[1], 1);
	else
	{
		fd = open(pipex->argv[pipex->argc - 1], O_WRONLY | O_CREAT | O_APPEND,
				0644);
		if (fd == -1)
		{
			perror("open outfile");
			exit(1);
		}
		dup2(fd, 1);
		close(fd);
	}
	close(pipex->fds[0]);
	close(pipex->fds[1]);
}

void	handle_heredoc_child(int i, t_pipex *pipex)
{
	char	**cmd;

	setup_heredoc_child(i, pipex);
	cmd = ft_split(pipex->argv[i + 3], ' ');
	if (!cmd || !cmd[0])
	{
		free_split(cmd);
		perror("Command not found");
		exit(127);
	}
	execute_command(cmd, pipex->envp);
}

int	setup_heredoc_pipe(int *fds, char *limiter)
{
	int	pid;

	if (pipe(fds) == -1)
		return (perror("pipe"), 1);
	pid = fork();
	if (pid == -1)
		return (perror("fork"), 1);
	if (pid == 0)
	{
		close(fds[0]);
		handle_heredoc_input(limiter, fds[1]);
		exit(0);
	}
	close(fds[1]);
	wait(NULL);
	return (0);
}

int	process_heredoc_commands(t_pipex *pipex)
{
	int	pid;
	int	i;
	int	last_pid;
	int	status;

	i = 0;
	while (i < pipex->argc - 4)
	{
		if (i != pipex->argc - 5 && pipe(pipex->fds) == -1)
			return (perror("pipe"), 1);
		pid = fork();
		if (pid == -1)
			return (perror("fork"), 1);
		if (pid == 0)
			handle_heredoc_child(i, pipex);
		last_pid = pid;
		close(pipex->prev_pipe);
		pipex->prev_pipe = pipex->fds[0];
		if (i++ != pipex->argc - 5)
			close(pipex->fds[1]);
	}
	while (waitpid(last_pid, &status, 0) > 0)
		;
	return (WEXITSTATUS(status));
}

int	here_doc(int ac, char **av, char **env)
{
	t_pipex	pipex;

	if (ac < 6)
		return (ft_putstr_fd("Enter all the arguments\n", 2), 1);
	pipex.argc = ac;
	pipex.argv = av;
	pipex.envp = env;
	if (setup_heredoc_pipe(pipex.fds, av[2]) == 1)
		return (1);
	pipex.prev_pipe = pipex.fds[0];
	return (process_heredoc_commands(&pipex));
}
