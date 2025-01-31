/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ilkaddou <ilkaddou@42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/27 21:11:44 by ilkaddou          #+#    #+#             */
/*   Updated: 2025/01/31 01:17:05 by ilkaddou         ###   ########.fr       */
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
		free(tab[i++]);
	free(tab);
}

void	error_exit(char *msg)
{
	ft_putendl_fd(msg, 2);
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
			return (path);
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
		return (ft_putstr_fd("Error: PATH not found\n", 2), NULL);
	split = ft_split(env[i] + 5, ':');
	if (!split)
		error_exit("Memory allocation failed in command_path\n");
	if (access(command, F_OK) == 0)
		return (ft_free_tab(split), command);
	path = try_paths(split, command);
	ft_free_tab(split);
	if (!path)
		return (ft_putstr_fd("Error: Command not found\n", 2), NULL);
	return (path);
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
		exit(EXIT_FAILURE);
	}
	execve(path, command, env);
	free(path);
	ft_free_tab(command);
	error_exit("Execve failed\n");
}
