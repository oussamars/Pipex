/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oboussel <oboussel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/24 11:41:20 by oboussel          #+#    #+#             */
/*   Updated: 2025/02/24 13:55:57 by oboussel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

void	error_child(char *av)
{
	ft_putstr_fd("zsh: no such file or directory: ", 2);
	ft_putendl_fd(av, 2);
	exit(1);
}

void	permission_case(char **cmd)
{
	free_split(cmd);
	ft_putstr_fd("zsh: permission denied: \n", 2);
	exit(126);
}

void	free_split(char **split)
{
	int	i;

	i = 0;
	while (split[i])
		free(split[i++]);
	free(split);
}

char	*slash_cmd(char *cmd)
{
	if (access(cmd, F_OK | X_OK) == 0)
		return (ft_strdup(cmd));
	return (NULL);
}
