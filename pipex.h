#ifndef PIPEX_H
#define PIPEX_H

#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <stddef.h>

char	**ft_split(char const *s, char c);
int ft_strcmp(const char *s1, const char *s2);
void free_split(char **split);
char *check_path(char *cmd, char **env);
int	ft_strncmp(const char *s1, const char *s2, size_t n);
int here_doc(int ac, char **av, char **env);

#endif