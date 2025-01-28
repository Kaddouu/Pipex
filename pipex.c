/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ilkaddou <ilkaddou@42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/27 21:11:39 by ilkaddou          #+#    #+#             */
/*   Updated: 2025/01/28 21:34:22 by ilkaddou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

void	child_proc(char **av, char **env, int *fd)
{
	int	file1;

	file1 = open(av[1], O_RDONLY, 0777);
	if (file1 == -1)
	{
		perror(errno);
		return ;
	}
	dup2(file1, STDIN_FILENO);
	dup2(fd[1], STDOUT_FILENO);
	close(fd[0]);
	exec(av[2], env);
}
void	parent_proc(char **av, char **env, int *fd)
{
	int	file2;

	file2 = open(av[4], O_RDONLY | O_WRONLY | O_TRUNC | O_CREAT, 0777);
	if (file2 == -1)
	{
		perror(errno);
		return ;
	}
	dup2(fd[0], STDIN_FILENO);
	dup2(file2, STDOUT_FILENO);
	close(fd[1]);
	exec(av[3], env);
}


int	main(int ac, char **av, char **env)
{
	pid_t	pid;
	int		fd[2];
	
	if (ac == 5)
	{
		if (pipe(fd) == -1)
		{
			perror(errno);
			return (1);
		}
		pid = fork();
		if (pid < 0)
		{
			perror(errno);
			return (1);	
		}
		if (pid == 0)
			child_proc(av, env, fd);
		waitpid(pid, NULL, 0);
		parent_proc(av, env, fd);
	}
	else
		ft_printf("Error\nBad arguments\n");
	return (0);
}
