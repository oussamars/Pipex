/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_child_bonus.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oboussel <oboussel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/25 10:44:11 by oboussel          #+#    #+#             */
/*   Updated: 2025/02/26 10:31:26 by oboussel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex_bonus.h"

void	open_infile(t_pipex *pipex)
{
	int	fd;

	fd = open(pipex->argv[1], O_RDONLY);
	if (fd == -1)
	{
		perror("open infile");
		exit(1);
	}
	dup2(fd, 0);
	dup2(pipex->fds[1], 1);
	close(fd);
}

void	open_outfile(t_pipex *pipex)
{
	int	fd;

	if (ft_strcmp(pipex->argv[pipex->argc - 1], "/dev/stdout") == 0)
		fd = 1;
	else
		fd = open(pipex->argv[pipex->argc - 1], O_WRONLY | O_CREAT | O_TRUNC,
				0644);
	if (fd == -1)
	{
		perror("open outfile");
		exit(1);
	}
	dup2(pipex->prev_pipe, 0);
	dup2(fd, 1);
	if (fd != 1)
		close(fd);
}

void	setup_child_io(int i, t_pipex *pipex)
{
	if (i == 0)
		open_infile(pipex);
	else if (i == pipex->argc - 4)
		open_outfile(pipex);
	else
	{
		dup2(pipex->prev_pipe, 0);
		dup2(pipex->fds[1], 1);
	}
	close(pipex->fds[0]);
	close(pipex->fds[1]);
}

void	handle_child(int i, t_pipex *pipex)
{
	char	**cmd;

	setup_child_io(i, pipex);
	cmd = ft_split(pipex->argv[i + 2], ' ');
	if (!cmd || !cmd[0])
	{
		free_split(cmd);
		perror("command not found");
		exit(127);
	}
	execute_command(cmd, pipex->envp);
}
