/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_bonus.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oboussel <oboussel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/24 15:46:37 by oboussel          #+#    #+#             */
/*   Updated: 2025/02/25 11:46:33 by oboussel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex_bonus.h"

int	setup_main_pipe(int i, t_pipex *pipex)
{
	int	pid;

	if (i != pipex->argc - 4 && pipe(pipex->fds) == -1)
		return (perror("pipe"), 1);
	pid = fork();
	if (pid == -1)
		return (perror("fork"), 1);
	if (pid == 0)
		handle_child(i, pipex);
	if (pipex->prev_pipe != -1)
		close(pipex->prev_pipe);
	pipex->prev_pipe = pipex->fds[0];
	close(pipex->fds[1]);
	return (0);
}

int	process_main_commands(t_pipex *pipex)
{
	int	last_pid;
	int	status;
	int	i;

	status = 0;
	pipex->prev_pipe = -1;
	i = 0;
	while (i < pipex->argc - 3)
	{
		if (setup_main_pipe(i, pipex) == 1)
			return (1);
		last_pid = i;
		i++;
	}
	while (waitpid(last_pid, &status, 0) > 0)
		;
	return (WEXITSTATUS(status));
}

int	main(int ac, char **av, char **env)
{
	t_pipex	pipex;

	if (ac < 5)
		return (ft_putstr_fd("Enter all the arguments\n", 2), 1);
	pipex.argc = ac;
	pipex.argv = av;
	pipex.envp = env;
	if (ft_strcmp("here_doc", av[1]) == 0)
		return (here_doc(ac, av, env));
	return (process_main_commands(&pipex));
}
