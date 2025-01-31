/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ilkaddou <ilkaddou@42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/27 21:11:39 by ilkaddou          #+#    #+#             */
/*   Updated: 2025/01/31 01:20:20 by ilkaddou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

void	child_proc(char **av, char **env, int *fd)
{
	int	file1;

	file1 = open(av[1], O_RDONLY);
	if (file1 < 0)
	{
		close(fd[0]);
		close(fd[1]);
		error_exit("Failed to open input file");
	}
	if (dup2(file1, STDIN_FILENO) < 0 || dup2(fd[1], STDOUT_FILENO) < 0)
	{
		close(file1);
		close(fd[0]);
		close(fd[1]);
		error_exit("Child: Dup2 failed\n");
	}
	close(file1);
	close(fd[0]);
	close(fd[1]);
	exec(av[2], env);
}

void	parent_proc(char **av, char **env, int *fd)
{
	int	file2;

	file2 = open(av[4], O_WRONLY | O_TRUNC | O_CREAT, 0777);
	if (file2 < 0)
	{
		close(fd[0]);
		close(fd[1]);
		error_exit("Failed to open output file");
	}
	if (dup2(fd[0], STDIN_FILENO) < 0 || dup2(file2, STDOUT_FILENO) < 0)
	{
		close(file2);
		close(fd[0]);
		close(fd[1]);
		error_exit("Parent: Dup2 failed");
	}
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
	if (pipe(fd) < 0)
		error_exit("Pipe failed\n");
	pid = fork();
	if (pid < 0)
	{
		close(fd[0]);
		close(fd[1]);
		error_exit("Fork failed\n");
	}
	if (pid == 0)
		child_proc(av, env, fd);
	waitpid(pid, NULL, 0);
	parent_proc(av, env, fd);
	close(fd[0]);
	close(fd[1]);
	return (0);
}
