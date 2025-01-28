/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ilkaddou <ilkaddou@42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/27 21:11:39 by ilkaddou          #+#    #+#             */
/*   Updated: 2025/01/28 21:47:45 by ilkaddou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

void	child_proc(char **av, char **env, int *fd)
{
	int	file1;

	if (!av || !env || !fd)
		error_exit("Invalid arguments\n");
	file1 = open(av[1], O_RDONLY);
	if (file1 == -1)
	{
		perror("Error");
		exit(EXIT_FAILURE);
	}
	if (dup2(file1, STDIN_FILENO) == -1)
		error_exit("Dup2 failed\n");
	if (dup2(fd[1], STDOUT_FILENO) == -1)
		error_exit("Dup2 failed\n");
	close(file1);
	close(fd[0]);
	close(fd[1]);
	exec(av[2], env);
}

void	parent_proc(char **av, char **env, int *fd)
{
	int	file2;

	if (!av || !env || !fd)
		error_exit("Invalid arguments\n");
	file2 = open(av[4], O_WRONLY | O_TRUNC | O_CREAT, 0777);
	if (file2 == -1)
	{
		perror("Error");
		exit(EXIT_FAILURE);
	}
	if (dup2(fd[0], STDIN_FILENO) == -1)
		error_exit("Dup2 failed\n");
	if (dup2(file2, STDOUT_FILENO) == -1)
		error_exit("Dup2 failed\n");
	close(file2);
	close(fd[0]);
	close(fd[1]);
	exec(av[3], env);
}

int	main(int ac, char **av, char **env)
{
	int		fd[2];
	pid_t	pid;

	if (ac != 5)
		error_exit(ERR_ARGS);
	if (!env)
		error_exit(ERR_ENV);
	if (pipe(fd) == -1)
	{
		perror("Error");
		return (1);
	}
	pid = fork();
	if (pid < 0)
	{
		perror("Error");
		return (1);
	}
	if (pid == 0)
		child_proc(av, env, fd);
	waitpid(pid, NULL, 0);
	parent_proc(av, env, fd);
	return (0);
}
