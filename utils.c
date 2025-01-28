/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ilkaddou <ilkaddou@42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/27 21:11:44 by ilkaddou          #+#    #+#             */
/*   Updated: 2025/01/28 21:34:44 by ilkaddou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

char	*command_path(char *command, char **env)
{
	char	**split;
	char	*temp;
	char	*path;
	int		i;
	
	i = 0;
	while (!ft_strnstr(env[i], "PATH", 4))
		i++;
	split = ft_split(env[i] + 5, ':');
	i = -1;
	while (split[++i])
	{
		temp = ft_strjoin(split[i], '/');
		path = ft_strjoin(temp, command);
		free(temp);
		if (access(path, F_OK))
			return (path);
		free(path);
	}
	i = -1;
	while (split[++i])
		free(split[i]);
	return (free(split), NULL);
}

void	exec(char *arg_cmd, char **env)
{
	int		i;
	char	*path;
	char	**command;
	i = -1;
	
	command = ft_split(arg_cmd, ' ');
	path = command_path(command, env);
	if (!path)
	{
		while (command[++i])
			free(command[i]);
		free(command);
	}
	if (execve(path, command, env) < 0)
	{
		perror(errno);
		exit(EXIT_FAILURE);
	}
}
