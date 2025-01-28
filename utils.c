/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ilkaddou <ilkaddou@42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/27 21:11:44 by ilkaddou          #+#    #+#             */
/*   Updated: 2025/01/28 21:47:07 by ilkaddou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

void	ft_free_tab(char **tab)
{
	int	i;

	i = 0;
	if (!tab)
		return ;
	while (tab[i])
	{
		free(tab[i]);
		i++;
	}
	free(tab);
}

void	error_exit(char *msg)
{
	ft_printf("Error: %s", msg);
	exit(EXIT_FAILURE);
}

static char	*try_paths(char **split, char *command)
{
	int		i;
	char	*temp;
	char	*path;

	i = -1;
	while (split[++i])
	{
		temp = ft_strjoin(split[i], "/");
		if (!temp)
		{
			ft_free_tab(split);
			error_exit("Memory allocation failed\n");
		}
		path = ft_strjoin(temp, command);
		free(temp);
		if (!path)
		{
			ft_free_tab(split);
			error_exit("Memory allocation failed\n");
		}
		if (access(path, F_OK | X_OK) == 0)
		{
			ft_free_tab(split);
			return (path);
		}
		free(path);
	}
	return (NULL);
}

char	*command_path(char *command, char **env)
{
	char	**split;
	char	*path;
	int		i;

	if (!command || !env)
		error_exit(ERR_CMD);
	i = 0;
	while (env[i] && !ft_strnstr(env[i], "PATH", 4))
		i++;
	if (!env[i])
		error_exit(ERR_ENV);
	split = ft_split(env[i] + 5, ':');
	if (!split)
		error_exit("Memory allocation failed\n");
	path = try_paths(split, command);
	if (path)
		return (path);
	ft_free_tab(split);
	return (NULL);
}

void	exec(char *arg_cmd, char **env)
{
	char	**command;
	char	*path;

	if (!arg_cmd || !env)
		error_exit(ERR_CMD);
	command = ft_split(arg_cmd, ' ');
	if (!command || !command[0])
	{
		ft_free_tab(command);
		error_exit(ERR_CMD);
	}
	path = command_path(command[0], env);
	if (!path)
	{
		ft_free_tab(command);
		error_exit(ERR_CMD);
	}
	execve(path, command, env);
	free(path);
	ft_free_tab(command);
	perror("Error");
	exit(EXIT_FAILURE);
}
