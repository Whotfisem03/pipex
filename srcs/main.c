/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: engo <engo@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/24 12:36:14 by engo              #+#    #+#             */
/*   Updated: 2022/11/24 15:11:39 by engo             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/pipex.h"

void	main(int ac, char **av)
{
	int		fd[2];
	pid_t	id;

	if (pipe(fd) == -1)
	{
		perror("pipe");
		exit(EXIT_FAILURE);
	}
	id = fork();
	if (id == -1)
	{
		perror("pipe");
		exit(EXIT_FAILURE);
	}
	if (id == 0)
	{
		close (fd[1]);
		dup2(fd[1], stdout);
	}
	waitpid();
}
