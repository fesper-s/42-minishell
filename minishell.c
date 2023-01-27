/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fesper-s <fesper-s@student.42.rio>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/04 13:51:41 by fesper-s          #+#    #+#             */
/*   Updated: 2023/01/26 18:38:48 by fesper-s         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	g_status;

void	expand_var(t_line **line, t_env *env)
{
	if ((*line)->cmds[0][0] == '$')
	{
		if (!(*line)->cmds[0][1])
			cmd_error((*line)->cmds[0]);
		else if ((*line)->cmds[0][1] == '?')
		{
			printf("minishell: %d", g_status);
			if ((*line)->cmds[0][2] != 0)
				printf("%s", &(*line)->cmds[0][2]);
			printf(": command not found\n");
		}
		else
			expanding(line, env);
		g_status = 127;
	}
}

void	exec_cmds(t_line **line, pid_t pid, int *fdd, int *fd)
{
	char	*path;

	path = find_path(line);
	if (pid == 0)
	{
		dup2(*fdd, 0);
		if ((*line)->next != 0)
			dup2(fd[1], 1);
		close(fd[0]);
		execve(path, (*line)->cmds, (*line)->env);
	}
	else
	{
		close(fd[1]);
		*fdd = fd[0];
		(*line) = (*line)->next;
	}
	free(path);
}

void open_files(t_line **line, int *fd)
{
	if ((*line)->infile)
		fd[0] = open((*line)->infile, O_RDONLY);
	else if (fd[0] == -1)
		printf("Error: no such file or directory: %s", (*line)->infile);
	if ((*line)->outfile)
		fd[1] = open((*line)->outfile, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	else if (fd[1] == -1)
		printf("Error: no such file or directory: %s", (*line)->outfile);
}

void	pipeline(t_line **line, int size)
{
	int		fd[2];
	pid_t	pid;
	int		fdd;
	char	*path;

	fdd = 0;
	while ((*line))
	{
		fd[0] = 0;
		fd[1] = 0;
		open_files(line, fd);
		path = find_path(line);
		if (!path)
			break ;
		pipe(fd);
		pid = fork();
		if (pid == -1 && print_error("Error: fork\n"))
			break ;
		else
			exec_cmds(line, pid, &fdd, fd);
		free(path);
	}
	close(fd[0]);
	close(fd[1]);
	while (size--)
		waitpid(-1, NULL, 0);
}

void	cmd_process(t_line **line, t_env **env)
{
	int		isbuiltin;
	void	*head;
	int		size;

	size = ft_lst_size((*line));
	head = (*line);
	if (!(*line)->cmds[0])
		return ;
	expand_var(line, *env);
	isbuiltin = handle_builtins((*line)->cmds, env);
	while (*line)
	{
		(*line)->env = ft_strdupp((*env)->env);
		*line = (*line)->next;
	}
	*line = head;
	if (!isbuiltin && !check_dir((*line)->cmds, (*env)->env))
	{
		g_status = 0;
		pipeline(line, size);
		(*line) = head;
	}
}

void	minishell(char **envp)
{
	t_line	*line;
	t_env	*env;
	void	*head;

	env = malloc(sizeof(t_env));
	env->env = ft_strdupp(envp);
	while (1)
	{
		line = ft_lst_new(NULL, NULL, NULL);
		head = line;
		signals();
		line->cmd = readline("minishell % ");
		if (line->cmd)
			add_history(line->cmd);
		else
			break ;
		if (organize_line(&line))
		{
			if (ft_strncmp(line->cmd, "exit", 5) == 0)
				break ;
			cmd_process(&line, &env);
			line = head;
			lst_free(&line);
		}
		free(line);
	}
	free(line->cmd);
	free_charpp(line->cmds);
	if (line)
		free(line);
	int	i = -1;
	while (env->env[++i])
		free(env->env[i]);
	free(env->env);
	free(env);
	printf("exit\n");
}
