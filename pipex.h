/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oboussel <oboussel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/23 15:06:30 by oboussel          #+#    #+#             */
/*   Updated: 2025/02/24 13:54:49 by oboussel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PIPEX_H
# define PIPEX_H

# include "get_next_line/get_next_line.h"
# include <errno.h>
# include <fcntl.h>
# include <stddef.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <sys/wait.h>
# include <unistd.h>

char	**ft_split(char const *s, char c);
int		ft_strcmp(const char *s1, const char *s2);
void	free_split(char **split);
char	*check_path(char *cmd, char **env);
int		ft_strncmp(const char *s1, const char *s2, size_t n);
int		here_doc(int ac, char **av, char **env);
void	ft_putendl_fd(char *s, int fd);
void	ft_putstr_fd(char *s, int fd);
void	ft_putchar_fd(char c, int fd);
int		ft_strncmp(const char *s1, const char *s2, size_t n);
void	error_child(char *av);
void	permission_case(char **cmd);
char	*slash_cmd(char *cmd);

#endif