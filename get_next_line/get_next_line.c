/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oboussel <oboussel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/22 17:29:32 by oboussel          #+#    #+#             */
/*   Updated: 2025/02/24 13:55:43 by oboussel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

static char	*join_free(char *stock, char *buffer)
{
	char	*temp;

	temp = ft_strjoin(stock, buffer);
	free(stock);
	stock = NULL;
	return (temp);
}

char	*ft_strchr(const char *s, int c)
{
	int	i;

	i = 0;
	while (s[i] != '\0')
	{
		if (s[i] == (unsigned char)c)
			return ((char *)(s + i));
		i++;
	}
	if ((unsigned char)c == '\0')
		return ((char *)(s + i));
	return (NULL);
}

static char	*ft_join(char *stock, int fd)
{
	char	*buffer;
	ssize_t	bytes_read;

	if (!stock)
		stock = ft_strdup("");
	buffer = malloc((size_t)BUFFER_SIZE + 1);
	if (buffer == NULL)
		return (NULL);
	bytes_read = 1;
	while (bytes_read > 0)
	{
		bytes_read = read(fd, buffer, BUFFER_SIZE);
		if (bytes_read == -1)
		{
			free(stock);
			free(buffer);
			return (NULL);
		}
		buffer[bytes_read] = '\0';
		stock = join_free(stock, buffer);
		if (ft_strchr(buffer, '\n'))
			break ;
	}
	free(buffer);
	return (stock);
}

char	*free_if_empty(char *str)
{
	if (str && str[0] == '\0')
	{
		free(str);
		str = NULL;
	}
	return (str);
}

char	*get_next_line(int fd)
{
	static char	*stock;
	char		*res;
	char		*temp;
	int			i;

	i = 0;
	if (BUFFER_SIZE <= 0 || fd < 0)
		return (NULL);
	stock = ft_join(stock, fd);
	if (stock == NULL || stock[0] == '\0')
	{
		free(stock);
		stock = NULL;
		return (NULL);
	}
	while (stock[i] && stock[i] != '\n')
		i++;
	res = ft_substr(stock, 0, i + 1);
	temp = ft_substr(stock, i + 1, ft_strlen(stock) - i + 1);
	free(stock);
	stock = NULL;
	stock = free_if_empty(temp);
	return (res);
}
