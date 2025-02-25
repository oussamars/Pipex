/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_bonus.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oboussel <oboussel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/23 15:07:46 by oboussel          #+#    #+#             */
/*   Updated: 2025/02/25 11:46:37 by oboussel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PIPEX_BONUS_H
# define PIPEX_BONUS_H

# include "../get_next_line/get_next_line.h"
# include <errno.h>
# include <fcntl.h>
# include <stddef.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <sys/wait.h>
# include <unistd.h>

typedef struct s_pipex
{
	int		fds[2];
	int		prev_pipe;
	int		argc;
	char	**argv;
	char	**envp;
}			t_pipex;

char		**ft_split(char const *s, char c);
void		free_split(char **split);
void		execute_command(char **cmd, char **env);
int			here_doc(int ac, char **av, char **env);
void		ft_putendl_fd(char *s, int fd);
void		ft_putstr_fd(char *s, int fd);
int			ft_strncmp(const char *s1, const char *s2, size_t n);
void		handle_heredoc_input(char *limiter, int write_fd);
void		handle_child(int i, t_pipex *pipex);

#endif
