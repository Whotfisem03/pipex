/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: engo <engo@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/24 12:36:14 by engo              #+#    #+#             */
/*   Updated: 2022/12/05 19:11:45 by engo             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

char	*ft_path_cmd(char **path)
{
	int	i;

	i = 0;
	while (path[i] && ft_strncmp(path[i], "PATH=", 5))
		i++;
	return (path[i] + 5);
}

char	*find_cmd(char *path_cmd, char **exec)
{
	int		i;
	char	**list_path;
	char	*good_path;
	char	*full_path;
	char	*tmp;

	i = 0;
	list_path = ft_split(path_cmd, ':');
	check_exec(exec, list_path);
	if (access(exec[0], F_OK) == 0)
		return (exec[0]);
	while (list_path[i])
	{
		good_path = ft_strjoin(list_path[i], "/");
		tmp = ft_strdup(good_path);
		full_path = ft_strjoin(tmp, exec[0]);
		free(good_path);
		free(tmp);
		if (access(full_path, F_OK) == 0)
			return (full_path);
		free(full_path);
		i++;
	}
	ft_free_pipex(list_path);
	return ("");
}

void	ft_parent(int *pipefd, char **av, char **envp)
{
	int		fd;
	char	**exec;
	char	*cmd;
	char	*path;

	fd = open(av[4], O_CREAT | O_WRONLY | O_TRUNC, 0666);
	if (fd < 0)
	{
		perror("An error has occured.\n");
		exit(EXIT_FAILURE);
	}
	path = ft_path_cmd(envp);
	exec = ft_split(av[3], ' ');
	cmd = find_cmd(path, exec);
	dup2(fd, 1);
	close(pipefd[1]);
	dup2(pipefd[0], 0);
	if (fd == 1)
		exit(1);
	ft_execve(exec, cmd, envp);
}

void	ft_child(int *pipefd, char **av, char **envp)
{
	int		fd;
	char	**exec;
	char	*cmd;
	char	*path;

	fd = open(av[1], O_RDONLY);
	if (fd < 0)
	{
		perror("An error has occured.\n");
		exit(EXIT_FAILURE);
	}
	path = ft_path_cmd(envp);
	exec = ft_split(av[2], ' ');
	cmd = find_cmd(path, exec);
	dup2(fd, 0);
	close(pipefd[0]);
	dup2(pipefd[1], 1);
	if (fd == 0)
		exit(1);
	ft_execve(exec, cmd, envp);
}

int	main(int ac, char **av, char **envp)
{
	pid_t	id;
	pid_t	id2;
	int		fd[2];

	if (ac == 5)
	{
		if (pipe(fd) == -1)
			error_exit("Error, pipe failed.\n");
		id = fork();
		if (id == -1)
			error_exit("Error, fork failed.\n");
		if (id == 0)
			ft_child(fd, av, envp);
		id2 = fork();
		if (id2 == -1)
			error_exit("Error, fork failed.\n");
		if (id2 == 0)
			ft_parent(fd, av, envp);
		close_fd(fd);
		waitpid(id, NULL, 0);
		waitpid(id2, NULL, 0);
	}
	else
		error_exit("Error arguments.\n");
	return (0);
}
