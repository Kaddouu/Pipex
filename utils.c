/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ilkaddou <ilkaddou@42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/27 21:11:44 by ilkaddou          #+#    #+#             */
/*   Updated: 2025/01/31 10:20:22 by ilkaddou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

void	error_exit(char *msg)
{
	if (errno != 0)
		perror(msg);
	else
		ft_putendl_fd(msg, 2);
	exit(EXIT_FAILURE);
}

static char	*check_direct_path(char *cmd)
{
	char	*path;

	if (!cmd)
		return (NULL);
	if (cmd[0] == '/' || (cmd[0] == '.' && cmd[1] == '/'))
	{
		if (access(cmd, F_OK | X_OK) == 0)
		{
			path = ft_strdup(cmd);
			if (!path)
				error_exit("Memory allocation failed");
			return (path);
		}
	}
	return (NULL);
}

static char	*try_paths(char **paths, char *cmd)
{
	int		i;
	char	*temp;
	char	*path;

	i = -1;
	while (paths[++i])
	{
		temp = ft_strjoin(paths[i], "/");
		if (!temp)
		{
			ft_free_tab(paths);
			error_exit("Memory allocation failed");
		}
		path = ft_strjoin(temp, cmd);
		free(temp);
		if (!path)
		{
			ft_free_tab(paths);
			error_exit("Memory allocation failed");
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
	char	*direct_path;
	int		i;

	if (!command || !env)
		error_exit("Invalid command or environment");
	direct_path = check_direct_path(command);
	if (direct_path)
		return (direct_path);
	i = 0;
	while (env[i] && !ft_strnstr(env[i], "PATH=", 5))
		i++;
	if (!env[i])
		return (NULL);
	split = ft_split(env[i] + 5, ':');
	if (!split)
		error_exit("Memory allocation failed");
	path = try_paths(split, command);
	ft_free_tab(split);
	return (path);
}

void	exec(char *arg_cmd, char **env)
{
	char	**command;
	char	*path;

	if (!arg_cmd || !env)
		error_exit("Invalid command or environment");
	command = ft_split(arg_cmd, ' ');
	if (!command || !command[0])
	{
		ft_free_tab(command);
		error_exit("Empty command");
	}
	path = command_path(command[0], env);
	if (!path)
	{
		ft_putstr_fd("Command not found: ", 2);
		ft_putendl_fd(command[0], 2);
		ft_free_tab(command);
		exit(127);
	}
	if (execve(path, command, env) == -1)
	{
		free(path);
		ft_free_tab(command);
		error_exit("Execution failed");
	}
}
