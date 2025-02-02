/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ilkaddou <ilkaddou@42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/27 21:11:39 by ilkaddou          #+#    #+#             */
/*   Updated: 2025/02/02 20:08:42 by ilkaddou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

void	left_pipe(char **av, char **env, int *fd)
{
	int	file1;

	file1 = open(av[1], O_RDONLY);
	if (file1 < 0)
	{
		close(fd[0]);
		close(fd[1]);
		error_exit("Input file");
	}
	dup2(file1, STDIN_FILENO);
	dup2(fd[1], STDOUT_FILENO);
	close(fd[0]);
	close(fd[1]);
	close(file1);
	exec(av[2], env);
}

void	right_pipe(char **av, char **env, int *fd)
{
	int	file2;

	file2 = open(av[4], O_WRONLY | O_TRUNC | O_CREAT, 0777);
	if (file2 < 0)
	{
		close(fd[0]);
		close(fd[1]);
		error_exit("Output file");
	}
	dup2(fd[0], STDIN_FILENO);
	dup2(file2, STDOUT_FILENO);
	close(fd[0]);
	close(fd[1]);
	close(file2);
	exec(av[3], env);
}

int	main(int ac, char **av, char **env)
{
	int		fd[2];
	pid_t	pid1;
	pid_t	pid2;

	if (ac != 5)
		error_exit(ERR_ARGS);
	if (!env)
		error_exit(ERR_ENV);
	if (pipe(fd) == -1)
		error_exit("Pipe");
	pid1 = fork();
	if (pid1 == -1)
		error_exit("Fork");
	if (pid1 == 0)
		left_pipe(av, env, fd);
	pid2 = fork();
	if (pid2 == -1)
		error_exit("Fork");
	if (pid2 == 0)
		right_pipe(av, env, fd);
	close(fd[0]);
	close(fd[1]);
	waitpid(pid1, NULL, 0);
	waitpid(pid2, NULL, 0);
	return (0);
}
