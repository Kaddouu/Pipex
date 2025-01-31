/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ilkaddou <ilkaddou@42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/27 20:59:35 by ilkaddou          #+#    #+#             */
/*   Updated: 2025/01/31 09:18:28 by ilkaddou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PIPEX_H
# define PIPEX_H

# include "ft_printf/ft_printf.h"
# include "libft/libft.h"
# include <errno.h>
# include <fcntl.h>
# include <string.h>
# include <sys/types.h>
# include <sys/wait.h>

# define ERR_CMD "Error: Command not found"
# define ERR_ENV "Error: Environment not found"
# define ERR_ARGS "Error: Invalid number of arguments"

void	child_proc(char **av, char **env, int *fd);
void	parent_proc(char **av, char **env, int *fd);
void	error_exit(char *msg);
void	ft_free_tab(char **tab);
void	exec(char *arg_cmd, char **env);
char	*command_path(char *command, char **env);
#endif
